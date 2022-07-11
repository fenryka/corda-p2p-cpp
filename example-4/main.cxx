#include <stdexcept>
#include <iostream>
#include <fstream>
#include <list>
#include <thread>
#include <memory>

#include <librdkafka/rdkafkacpp.h>
#include <nlohmann/json.hpp>

#include "config.h"
#include "Identity.h"
#include "CordaAvro.h"
#include "Message.h"

/**********************************************************************************************************************/

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

        if (auto err = consumer->subscribe({ "p2p.in" }); err != RdKafka::ERR_NO_ERROR) {
            std::cerr << "Failed to subscribe" << std::endl;
            return;
        }

        for (;;) {
            if (auto msg = std::unique_ptr<RdKafka::Message>{  consumer->consume(500) }; msg->err() != RdKafka::ERR_NO_ERROR) {
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

    std::cout << "Starting consumer thread" << std::endl;
    std::thread consumer (Consumer(), conf);

    // Create the Producer instance.
    auto producer = RdKafka::Producer::create(conf, errstr);
    if (!producer) {
        std::stringstream ss;
        ss << "Failed to create new producer: " << errstr << std::endl;
        throw std::runtime_error (ss.str());
    } else {
        std::cout << "Producer created" << std::endl;
    }


    auto ktopic = RdKafka::Topic::create (producer, "p2p.out", nullptr, errstr);

    if (!ktopic) {
        std::stringstream ss;
        ss << "Failed to create topic p2p.out, " << errstr;
        throw std::runtime_error (ss.str());
    } else {
        std::cout << "Created topic " << "p2p.out" << std::endl;
    }

    std::string key = jconf["name"];
    auto me = corda::p2p::identity::Identity(jconf["x500name"]);
    auto them = corda::p2p::identity::Identity(jconf["to"]);

    for (std::string line; std::getline(std::cin, line);) {
        std::cout << "Msg to send: \"" << line << "\"" << std::endl;

        std::vector<unsigned char> v (line.begin(), line.end());
        corda::p2p::messaging::AppMessage am (v, me, them);

        auto appMessageFactory = am.factory();
        auto encoder = appMessageFactory->encodeAsBinaryToMem();
        auto res = encoder->encode (am);
        res->flush();

        auto bytes = avro::snapshot(*res);

        corda::p2p::messaging::Envelope envelope (0, am.fingerprint(), bytes);
        auto envelopeFactory = envelope.factory();
        auto envEncoder = envelopeFactory->encodeAsBinaryToMem();
        auto envOut = envEncoder->encode (envelope);
        envOut->flush();
        auto envBytes = avro::snapshot(*envOut);

        auto err = producer->produce(ktopic, RdKafka::Topic::PARTITION_UA,
                                     RdKafka::Producer::RK_MSG_COPY,
                                     envBytes->data(), envBytes->size(),
                                     &key,
                                     nullptr);

        if (err != RdKafka::ERR_NO_ERROR) {
            throw std::runtime_error ("Failed to send");
        } else {
            std::cout << "Sent" << std::endl;
        }
    }

}

/**********************************************************************************************************************/
