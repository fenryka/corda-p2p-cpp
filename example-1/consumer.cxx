#include <glib.h>
#include <librdkafka/rdkafkacpp.h>

#include "common.cxx"

static volatile sig_atomic_t run = 1;

/**
 * @brief Signal termination of program
 */
static void stop([[maybe_unused]]int sig) {
    run = 0;
}

int main (int argc, char **argv) {
    std::string errstr;

    // Parse the command line.
    if (argc != 2) {
        g_error("Usage: %s <config.ini>", argv[0]);
    }

    const char *config_file = argv[1];

    g_autoptr(GKeyFile) key_file = g_key_file_new();
    {
        g_autoptr(GError) error = nullptr;
        if (!g_key_file_load_from_file(key_file, config_file, G_KEY_FILE_NONE, &error)) {
            g_error ("Error loading config file: %s", error->message);
        }
    }

    // Load the relevant configuration sections.
    auto conf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);
    load_config_group(*conf, *key_file, "default");
    load_config_group(*conf, *key_file, "consumer");

    // Create the Consumer instance.
    auto consumer = RdKafka::KafkaConsumer::create(conf,errstr);

    if (!consumer) {
        g_error("Failed to create new consumer: %s", errstr.c_str());
    }

    auto err = consumer->subscribe({ "my-topic-1"});

    if (err != RdKafka::ERR_NO_ERROR) {
        g_error("Failed to subscribe");
    }


    // Install a signal handler for clean shutdown.
    signal(SIGINT, stop);

    // Start polling for messages.
    while (run) {
        auto msg = consumer->consume(500);
        if (msg->err() != RdKafka::ERR_NO_ERROR) {
            if (msg->err() == RdKafka::ERR__TIMED_OUT) {
                g_message("Waiting...");
                continue;
            } else if (msg->err() == RdKafka::ERR__PARTITION_EOF) {
                /* We can ignore this error - it just means we've read
                   everything and are waiting for more data */
                continue;
            } else {
                g_message("Consumer error: %s", msg->errstr().c_str());
                return 1;
            }
       } else {

           g_message("Consumed event from topic %s: key = %s value = %s",
                     msg->topic_name().c_str(),
                     msg->key()->c_str(),
                     (char *)msg->payload()
           );
       }

        // Free the message when we're done.
        delete (msg);
    }

    // Close the consumer: commit final offsets and leave the group.
    g_message( "Closing consumer");
    consumer->close();

    return 0;
}
