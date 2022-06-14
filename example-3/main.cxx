#include <stdexcept>
#include <iostream>
#include <fstream>
#include <list>
#include <thread>

#include <librdkafka/rdkafkacpp.h>
#include <nlohmann/json.hpp>

#include "config.h"

std::list<std::string> Identities = { "alice", "bob", "claire" }; // NOLINT

/*
 * Listens to this identities topic for messages
 */
class Consumer {
public :
    void operator()(RdKafka::Conf * config_, const std::string & topic_) {
        std::string errstr;
        auto consumer = RdKafka::KafkaConsumer::create (config_, errstr);

        if (!consumer) {
            std::cerr << "Failed to create new consumer: " << errstr << std::endl;
            return;
        }

        auto err = consumer->subscribe({topic_});

        if (err != RdKafka::ERR_NO_ERROR) {
            std::cerr << "Failed to subscribe" << std::endl;
            return;
        }

        for (;;) {
            auto msg = consumer->consume(500);
            if (msg->err() != RdKafka::ERR_NO_ERROR) {
                if (msg->err() == RdKafka::ERR__TIMED_OUT) {
                    continue;
                } else if (msg->err() == RdKafka::ERR__PARTITION_EOF) {
                    /* We can ignore this error - it just means we've read
                       everything and are waiting for more data */
                    continue;
                } else {
                    std::cerr << "Consumer error: " << msg->errstr() << std::endl;
                    return;
                }
            } else {
                std::cout
                        << "Consumed event. Key = " << *msg->key()
                        << " value = " << (char *)msg->payload() << std::endl;
            }

            // Free the message when we're done.
            delete (msg);
        }
    }
};

int
main (int argc, char ** argv) {
    std::string errstr;

    if (argc != 3) {
        throw std::invalid_argument("No identity provided, no config provided");
    }

    const std::string identity { *(argv + 1) };
    if (std::find (Identities.begin(), Identities.end(), identity) == Identities.end()) {
        std::stringstream ss;
        ss << identity << " is not a valid identity";
        throw std::invalid_argument(ss.str());
    }

    std::ifstream config_file (*(argv + 2));
    nlohmann::json jconf;
    config_file >> jconf;

    // Load the relevant configuration sections.
    auto conf = corda::p2p::config::parse("default", jconf);
    corda::p2p::config::parse (*conf, identity, jconf);


    std::thread consumer (Consumer(), conf, jconf[identity]["topic-in"]);


    // Create the Producer instance.
    auto producer = RdKafka::Producer::create(conf, errstr);
    if (!producer) {
        std::stringstream ss;
        ss << "Failed to create new producer: " << errstr << std::endl;
        throw std::runtime_error (ss.str());
    }


    std::map<std::string, RdKafka::Topic *> topics;

    for (const auto & topic : jconf[identity]["topic-out"]) {
        auto ktopic = RdKafka::Topic::create(producer, topic, nullptr, errstr);

        if (!ktopic) {
            std::stringstream ss;
            ss << "Failed to create toppic: " << topic << ", err: " << errstr;
            throw std::runtime_error (ss.str());
        }

        topics.emplace(std::string (topic), ktopic);
    }

    for (std::string line; std::getline(std::cin, line);) {

        for (const auto & topicPair : topics) {
            auto err = producer->produce(topicPair.second, RdKafka::Topic::PARTITION_UA,
                                         RdKafka::Producer::RK_MSG_COPY,
                                         (void *)line.c_str(), line.size(),
                                         &topicPair.first,
                                         nullptr
            );

            if (err != RdKafka::ERR_NO_ERROR) {
                throw std::runtime_error ("Failed to send");
            }
        }
    }

}