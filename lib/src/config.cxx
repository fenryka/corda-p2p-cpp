#include "config.h"

#include <nlohmann/json.hpp>
#include <librdkafka/rdkafkacpp.h>
#include <iostream>
#include <sstream>

#include "debug.h"

/**********************************************************************************************************************/

void
corda::p2p::config::
parse (
        RdKafka::Conf & kconf_,
        const std::string & key_,
        const nlohmann::json & config_
) {

    if (!config_.contains("kafka")) {
        throw std::runtime_error ("Config file contains no kafka settings");
    }

    if (!config_["kafka"].contains(key_)) {
        std::stringstream ss;
        ss << "Cannot find key: \"" << key_ << "\"" << std::endl;
        DBG (ss.str()); // NOLINT

        throw std::runtime_error ("Cannot find key");
    }

    for (auto & [k, v]: config_["kafka"][key_].items()) {
        std::string err;
        auto result = kconf_.set(k, v, err);

        if (result != RdKafka::Conf::ConfResult::CONF_OK) {
            throw std::runtime_error (err);
        }
    }
}

/**********************************************************************************************************************/

RdKafka::Conf *
corda::p2p::config::
parse (
        const std::string & key_,
        const nlohmann::json & config_
) {
    auto kconf = RdKafka::Conf::create(RdKafka::Conf::CONF_GLOBAL);

    parse(*kconf, key_, config_);
    return kconf;
}

/**********************************************************************************************************************/
