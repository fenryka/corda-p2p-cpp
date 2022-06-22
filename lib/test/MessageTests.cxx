#include <gtest/gtest.h>
#include <Compiler.hh>
#include <avro/Schema.hh>

#include "X500.h"
#include "Message.h"
#include "Identity.h"
#include "X500Support.h"
#include "../src/SchemaBuilder.h"

/**********************************************************************************************************************/

using namespace corda::p2p::identity;
using namespace corda::p2p::messaging;

/**********************************************************************************************************************/

class MessageTests: public ::testing::Test {
public:
    avro::ValidSchema schema;

    MessageTests()
        : schema (buildAppMessageSchema())
    {

    }

    void SetUp( ) override {
    }

    void TearDown() override {
    }

    ~MessageTests() override = default;
};

/**********************************************************************************************************************/

TEST_F (MessageTests, basic) { // NOLINT
    auto aliceX500 = std::make_unique<X500> (test::Alice{});
    auto alice = Identity { aliceX500 };
    std::string key = "alice";
    std::string payload = "payload";
    auto message = Message { schema, key, payload, alice };
     try {
         message.encode(Identity("CN=Kat, L=London"));
     } catch (const std::exception & e) {
         std::cout << e.what() << std::endl;
     }
    ASSERT_NO_THROW(message.encode(Identity("CN=Kat, L=London")));
}

/**********************************************************************************************************************/

