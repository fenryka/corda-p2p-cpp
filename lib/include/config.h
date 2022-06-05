#pragma once

#include <nlohmann/json_fwd.hpp>

/*
 * Forward declare the Kafka config object
 */
namespace RdKafka {
    class Conf;
}

namespace corda::p2p::config {

    RdKafka::Conf * parse (const std::string &, const nlohmann::json &);
    void parse (RdKafka::Conf &, const std::string &, const nlohmann::json &);

}