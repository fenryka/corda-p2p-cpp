#include <gtest/gtest.h>
#include <Compiler.hh>

#include <fstream>

#include "X500.h"
#include "Message.h"
#include "Identity.h"
#include "X500Support.h"

/**********************************************************************************************************************/

using namespace corda::p2p::identity;
using namespace corda::p2p::messaging;

/**********************************************************************************************************************/


class MessageTests: public ::testing::Test {
public:
    avro::ValidSchema schema;

    MessageTests() = default;

    void SetUp( ) override {
        std::ifstream ifs { "../src/avro.asvc" };
        avro::compileJsonSchema (ifs, schema);
    }

    void TearDown() override {
    }

    ~MessageTests() override = default;
};

TEST_F (MessageTests, basic) { // NOLINT
    auto aliceX500 = std::make_unique<X500> (test::Alice{});
    auto alice = Identity { aliceX500 };
    std::string key = "alice";
    std::string payload = "payload";
    auto message = Message { schema, key, payload, alice };
    ASSERT_NO_THROW(message.encode(Identity("CN=Kat, L=London")));
}

/**********************************************************************************************************************/

