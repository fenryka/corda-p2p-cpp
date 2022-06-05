#include <librdkafka/rdkafkacpp.h>
#include <iostream>
#include <sstream>
#include <nlohmann/json.hpp>

#if defined __linux__
    #include <bsd/stdlib.h> // NOLINT - shut clang up about depricated headers
#include <fstream>

#endif

#include "config.h"

#define ARR_SIZE(arr) ( sizeof((arr)) / sizeof((arr[0])) )

class DR_MSG_CB : public RdKafka::DeliveryReportCb {
public :
    void dr_cb (RdKafka::Message &message) override {
        if (message.err()) {
            std::cerr << "Message delivery failed: " << message.errstr() << std::endl;
        }
    }
};

int main (int argc, char **argv) {
    std::string errstr;

    if (argc != 2) {
        std::cerr << "Usage: " << *argv << " <config.json>" << std::endl;
        return EXIT_FAILURE;
    }

    std::ifstream config_file (*(argv + 1));
    nlohmann::json jconf;
    config_file >> jconf;
    auto conf = corda::p2p::config::parse("default", jconf);

    // Install a delivery-error callback
    DR_MSG_CB dr_msg_cb;
    conf->set ("dr_cb", &dr_msg_cb, errstr);

    // Create the Producer instance.
    auto producer = RdKafka::Producer::create(conf, errstr);
    if (!producer) {
        std::stringstream ss;
        ss << "Failed to create new producer: " << errstr << std::endl;
        throw std::runtime_error (ss.str());
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
            std::cerr << "FUCK" << std::endl;
        }

        producer->poll(0);
    }

    // Block until the messages are all sent.
    std::cout << "Flushing final messages.." << std::endl;
    producer->flush(10 * 1000);
    if (producer->outq_len() > 0) {
        std::cerr << producer->outq_len() << "message(s) were not delivered" << std::endl;
    }

    std::cout << message_count << " events were produced to topic " << topic << "." << std::endl;

    return 0;
}
