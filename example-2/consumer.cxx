#include <glib.h>
#include <librdkafka/rdkafkacpp.h>
#include <fstream>
#include <nlohmann/json.hpp>

#include "config.h"

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
        g_error("Usage: %s <config.json>", argv[0]);
    }

    std::ifstream config_file (*(argv + 1));
    nlohmann::json jconf;
    config_file >> jconf;

    // Load the relevant configuration sections.
    auto conf = corda::p2p::config::parse("default", jconf);
    corda::p2p::config::parse (*conf, "consumer", jconf);

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
