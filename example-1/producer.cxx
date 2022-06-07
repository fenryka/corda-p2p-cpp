#include <glib.h>
#include <librdkafka/rdkafkacpp.h>
#include <iostream>

#if defined __linux__
    #include <bsd/stdlib.h> // NOLINT - shut clang up about depricated headers
#endif

#include "common.cxx"

#define ARR_SIZE(arr) ( sizeof((arr)) / sizeof((arr[0])) )

/* Optional per-message delivery callback (triggered by poll() or flush())
 * when a message has been successfully delivered or permanently
 * failed delivery (after retries).
 */

class DR_MSG_CB : public RdKafka::DeliveryReportCb {
public :
    void dr_cb (RdKafka::Message &message) override {
        if (message.err()) {
            g_error("Message delivery failed: %s", message.errstr().c_str());
        }
    }
};

int main (int argc, char **argv) {
    std::string errstr;

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
    auto conf = RdKafka::Conf::create(RdKafka::Conf::ConfType::CONF_GLOBAL);
    load_config_group(*conf, *key_file, "default");

    // Install a delivery-error callback
    DR_MSG_CB dr_msg_cb;
    if (conf->set ("dr_cb", &dr_msg_cb, errstr) != RdKafka::Conf::CONF_OK) {
        g_error ("Failed to set dr_cb - %s", errstr.c_str());
    }

    // Create the Producer instance.
    auto producer = RdKafka::Producer::create(conf, errstr);
    if (!producer) {
        g_error("Failed to create new producer: %s", errstr.c_str());
    }

    // Configuration object is now owned, and freed, by the rd_kafka_t instance.
    conf = nullptr;

    // Produce data by selecting random values from these lists.
    int message_count = 10;
    const char *topic = "my-topic-1";
    const char *user_ids[6] = {"eabara", "jsmith", "sgarcia", "jbernard", "htanaka", "awalther"};
    const char *products[5] = {"book", "alarm clock", "t-shirts", "gift card", "batteries"};

    for (int i = 0; i < message_count; i++) {
        const char *key =  user_ids[arc4random() % ARR_SIZE(user_ids)];
        const char *value =  products[arc4random() % ARR_SIZE(products)];
        size_t key_len = strlen(key);
        size_t value_len = strlen(value);

        auto ktopic = RdKafka::Topic::create(producer, topic, conf, errstr);
        auto err = producer->produce(ktopic, RdKafka::Topic::PARTITION_UA,
                          RdKafka::Producer::RK_MSG_COPY,
                                     (void *)value, value_len,
                                     (const void *)key, key_len,
                          nullptr
        );

        if (err != RdKafka::ERR_NO_ERROR) {
            g_error ("Produce failed - %i", err);
        }

        producer->poll(0);
    }

    // Block until the messages are all sent.
    g_message("Flushing final messages..");
    producer->flush(10 * 1000);
    if (producer->outq_len() > 0) {
        g_error("%d message(s) were not delivered", producer->outq_len());
    }

    g_message("%d events were produced to topic %s.", message_count, topic);

    return 0;
}
