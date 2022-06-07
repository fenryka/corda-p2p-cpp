#include <librdkafka/rdkafkacpp.h>
#include <fstream>
#include <iostream>
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
        std::cerr << "Usage: " << *argv << " <config.json>" << std::endl;
        return EXIT_FAILURE;
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
        std::cerr << "Failed to create new consumer: " << errstr << std::endl;
        return EXIT_FAILURE;
    }

    auto err = consumer->subscribe({ "my-topic-1"});

    if (err != RdKafka::ERR_NO_ERROR) {
        std::cerr << "Failed to subscribe" << std::endl;
        return EXIT_FAILURE;
    }

    // Install a signal handler for clean shutdown.
    signal(SIGINT, stop);

    // Start polling for messages.
    while (run) {
        auto msg = consumer->consume(500);
        if (msg->err() != RdKafka::ERR_NO_ERROR) {
            if (msg->err() == RdKafka::ERR__TIMED_OUT) {
                std::cout << "Waiting..." << std::endl;
                continue;
            } else if (msg->err() == RdKafka::ERR__PARTITION_EOF) {
                /* We can ignore this error - it just means we've read
                   everything and are waiting for more data */
                continue;
            } else {
                std::cerr << "Consumer error: " << msg->errstr() << std::endl;
                return 1;
            }
       } else {
            std::cout
                << "Consumed event from topic "
                << msg->topic_name() << ": key = " << msg->key()
                << " value = " << (char *)msg->payload() << std::endl;
       }

        // Free the message when we're done.
        delete (msg);
    }

    // Close the consumer: commit final offsets and leave the group.
    std::cout << "Closing consumer" << std::endl;
    consumer->close();

    return EXIT_SUCCESS;
}
