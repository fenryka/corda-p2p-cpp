#include <stdexcept>
#include <iostream>
#include <fstream>
#include <list>
#include <thread>
#include <memory>

#include <librdkafka/rdkafkacpp.h>
#include <nlohmann/json.hpp>

#include "config.h"
#include "MessageFactory.h"
#include "Identity.h"

/**********************************************************************************************************************/

auto from = std::string { "CN=Mark Smith, OU=Tivoli, O=IBM, L=Austin, S=Texas, C=US" };
auto to = std::string { "CN=Steve Meier, OU=Tivoli, O=IBM, C=US" };

int
main (int argc, char ** argv) {
    if (argc != 1) {
        throw std::invalid_argument("no arguments please");
    }

    auto me = corda::p2p::identity::Identity(from);
    auto them = corda::p2p::identity::Identity(to);

    auto factory = corda::p2p::messaging::MessageFactory("ignore me");
    int file { 0 };
    for (std::string line; std::getline(std::cin, line);) {
        std::stringstream ss;
        ss << "out" << file++ << ".json";
        auto message = factory.messageFileEncode(ss.str(), "THE KEY", line, me);
        auto val = message->encode(them);
        auto bytes = avro::snapshot(*val);

    }

}

/**********************************************************************************************************************/
