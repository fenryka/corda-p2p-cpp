#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include "config.h"

using json = nlohmann::json;

TEST (json, defaultSection) { // NOLINT
    auto config = R"({
        "default": {
            "bootstrap.servers" : "localhost:10002"
        },
        "consumer": {
            "group.id" : "kat.test.1"
        }
    })";

    auto jconf = json::parse(config);

    ASSERT_NO_THROW(corda::p2p::config::parse ("default", jconf));
}

TEST (json, emptyJson) { // NOLINT
    auto config = R"({})";

    auto jconf = json::parse(config);

    ASSERT_ANY_THROW(corda::p2p::config::parse ("default", jconf));
}

TEST (json, defaultSectionAndConsumer) { // NOLINT
    auto config = R"({
        "kafka": {
            "default": {
                "bootstrap.servers" : "localhost:10002"
            },
            "consumer": {
                "group.id" : "kat.test.1"
            }
        }
    })";

    auto jconf = json::parse(config);

    ASSERT_NO_THROW(corda::p2p::config::parse ("default", jconf));
}
