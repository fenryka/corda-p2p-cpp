#include <gtest/gtest.h>
#include <nlohmann/json.hpp>
#include <iostream>
#include "config.h"

using json = nlohmann::json;

TEST (typeName, test1) { // NOLINT

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