#include <stdexcept>
#include <iostream>
#include <fstream>
#include <list>
#include <thread>
#include <memory>

#include <librdkafka/rdkafkacpp.h>
#include <nlohmann/json.hpp>

#include "config.h"
#include "Message.h"
#include "Identity.h"

std::list<std::string> Identities = { "alice", "chip" }; // NOLINT

/**********************************************************************************************************************/

/*
 * Listens to this identities topic for messages
 */
class Consumer {
public :
    void operator()(RdKafka::Conf * config_) {
        std::string errstr;
        auto consumer = RdKafka::KafkaConsumer::create (config_, errstr);

        if (!consumer) {
            std::cerr << "Failed to create new consumer: " << errstr << std::endl;
            return;
        }

        auto err = consumer->subscribe({ "p2p.in" });

        if (err != RdKafka::ERR_NO_ERROR) {
            std::cerr << "Failed to subscribe" << std::endl;
            return;
        }

        for (;;) {
            auto msg = consumer->consume(500);
            if (msg->err() != RdKafka::ERR_NO_ERROR) {
                if (msg->err() == RdKafka::ERR__PARTITION_EOF || msg->err() == RdKafka::ERR__TIMED_OUT) {
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

/**********************************************************************************************************************/

int
main (int argc, char ** argv) {
    std::string errstr;

    if (argc != 2) {
        throw std::invalid_argument("No config provided");
    }

    std::ifstream config_file (*(argv + 1));
    nlohmann::json jconf;
    config_file >> jconf;

    // Load the relevant configuration sections.
    auto conf = corda::p2p::config::parse("default", jconf);
    corda::p2p::config::parse (*conf, "group", jconf);

    std::thread consumer (Consumer(), conf);

    // Create the Producer instance.
    auto producer = RdKafka::Producer::create(conf, errstr);
    if (!producer) {
        std::stringstream ss;
        ss << "Failed to create new producer: " << errstr << std::endl;
        throw std::runtime_error (ss.str());
    }


    auto ktopic = RdKafka::Topic::create(producer, "p2p.out", nullptr, errstr);

    if (!ktopic) {
        std::stringstream ss;
        ss << "Failed to create topic p2p.out, " << errstr;
        throw std::runtime_error (ss.str());
    }

    std::string key = jconf["name"];
    auto me = corda::p2p::identity::Identity(jconf["x500name"]);
    auto them = corda::p2p::identity::Identity(jconf["to"]);

    for (std::string line; std::getline(std::cin, line);) {

        auto message = corda::p2p::messaging::Message (line, key, me);

         auto val = message.encode(them);



         /*
                auto err = producer->produce(ktopic, RdKafka::Topic::PARTITION_UA,
                                     RdKafka::Producer::RK_MSG_COPY,
                                     val, val->byteCount(),
                                     &key,
                                     nullptr
        );

        if (err != RdKafka::ERR_NO_ERROR) {
            throw std::runtime_error ("Failed to send");
        }
                                     */
    }

}

/**********************************************************************************************************************/
