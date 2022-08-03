#include <stdexcept>
#include <iostream>

#include <nlohmann/json.hpp>

#include "CordaAvroSerializable.h"
#include "CordaAvroEncoder.h"
#include "Identity.h"

/**********************************************************************************************************************/

auto from = std::string { "CN=Mark Smith, OU=Tivoli, O=IBM, L=Austin, S=Texas, C=US" }; // NOLINT
auto to = std::string { "CN=Steve Meier, OU=Tivoli, O=IBM, C=US" }; // NOLINT

/**********************************************************************************************************************/

int
main (int argc, [[maybe_unused]] char ** argv) {
    if (argc != 1) {
        throw std::invalid_argument("no arguments please");
    }

    auto me = std::make_shared<corda::p2p::identity::Identity>(corda::p2p::identity::Identity(from));
    auto them = std::make_shared<corda::p2p::identity::Identity>(corda::p2p::identity::Identity(to));

    auto factory = corda::p2p::avro::CordaAvroFactory<corda::p2p::messaging::AppMessage>();

    int file { 0 };
    std::cout << "Enter text: ";
    for (std::string line; std::getline(std::cin, line);) {
        std::cout << " Entered - " << line << std::endl;
        std::stringstream ss;
        ss << "out-" << file++ << ".json";
        std::vector<unsigned char> v (line.begin(), line.end());
        corda::p2p::messaging::AppMessage am (v, me, them);
#if 0
        auto encoder = factory->encodeAsBinaryToFile(ss.str());
#else
        auto encoder = factory.encodeAsJSONToFile(ss.str());
#endif
        auto res = encoder->encode (am);
    }
}

/**********************************************************************************************************************/
