#include <stdexcept>
#include <iostream>

#include <nlohmann/json.hpp>

#include "Message.h"
#include "CordaAvro.h"
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

    auto me = corda::p2p::identity::Identity(from);
    auto them = corda::p2p::identity::Identity(to);

    int file { 0 };
    std::cout << "Enter text: ";
    for (std::string line; std::getline(std::cin, line);) {
        std::cout << " Entered - " << line << std::endl;
        std::stringstream ss;
        ss << "out-" << file++ << ".json";
        std::vector<unsigned char> v (line.begin(), line.end());
        corda::p2p::messaging::AppMessage am (v, me, them);
        auto factory = am.factory();
#if 0
        auto encoder = factory->encodeAsBinaryToFile(ss.str());
#else
        auto encoder = factory->encodeAsJSONToFile(ss.str());
#endif
        auto res = encoder->encode (am);
        res->flush();
    }
}

/**********************************************************************************************************************/
