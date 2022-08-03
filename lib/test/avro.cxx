#include <gtest/gtest.h>
#include <iostream>
#include <fstream>

#include <avro/Encoder.hh>
#include <avro/Decoder.hh>
#include <avro/Compiler.hh>
#include <avro/Schema.hh>

#include "../src/avro-manual.h"
#include "../src/SchemaBuilder.h"

#include "X500Support.h"

class AvroTests: public ::testing::Test {
public:
    avro::ValidSchema g_schema;

    AvroTests() = default;

    void SetUp( ) override {
        std::ifstream ifs { "avro.asvc" };
        avro::compileJsonSchema (ifs, g_schema);

      //  std::cout << "LOADED SCHEMA:" << g_schema.toJson(true);
    }

    void TearDown() override {
    }

    ~AvroTests() override = default;
};

/**********************************************************************************************************************/

/*
 * Stupid little test to just make sure Alice is who we need her to be
 */
TEST_F (AvroTests, alice) { // NOLINT
    corda::p2p::identity::test::Alice alice;
    ASSERT_EQ (alice.name(), "O=Alice, L=London, C=London");
}

/**********************************************************************************************************************/

TEST_F (AvroTests, test1) { // NOLINT
    corda::p2p::identity::test::Alice alice;
    net::corda::data::identity::HoldingIdentity hi;

    std::unique_ptr <avro::OutputStream> out = avro::memoryOutputStream();

    avro::EncoderPtr e = avro::binaryEncoder();
    e->init (*out);

    hi.x500Name = alice.name();
    avro::encode(*e, hi);

    std::unique_ptr<avro::InputStream> in = avro::memoryInputStream(*out);
    avro::DecoderPtr d = avro::binaryDecoder();
    d->init(*in);

    net::corda::data::identity::HoldingIdentity hi2 ;
    avro::decode(*d, hi2);

    std::cout << hi2.x500Name << std::endl;
}

/**********************************************************************************************************************/

TEST_F (AvroTests, unauthMsg) { // NOLINT
    net::corda::p2p::app::AppMessage am;
    net::corda::p2p::app::UnauthenticatedMessage um;
    net::corda::p2p::app::UnauthenticatedMessageHeader umh;
    net::corda::data::identity::HoldingIdentity from;
    net::corda::data::identity::HoldingIdentity to;

    corda::p2p::identity::test::Alice alice;
    corda::p2p::identity::test::Bob bob;

    from.x500Name = alice.name();
    to.x500Name = bob.name();

    umh.destination = to;
    umh.source = from;

    std::string payload = "hello bob";

    um.header = umh;
    std::vector<uint8_t> vec(payload.begin(), payload.end());
    um.payload = vec;

    am.message.set_UnauthenticatedMessage(um);
}

/**********************************************************************************************************************/

/**
 * Testing encoding Holding identity with just that in the schema
 */
TEST_F (AvroTests, holdingIdLS) { // NOLINT
    std::string hiSchema = {R"(
    {
        "type": "record",
        "name": "HoldingIdentity",
        "namespace": "net.corda.data.identity",
        "fields": [
            {
                "name": "x500Name",
                "type": "string"
            },
            {
                "name": "groupId",
                "type": "string"
            }
        ]
    })"
    };

    auto schema = avro::compileJsonSchemaFromString (hiSchema);

    std::unique_ptr <avro::OutputStream> out2 = avro::memoryOutputStream();

    avro::EncoderPtr e = avro::jsonEncoder (schema);
    e->init (*out2);

    net::corda::data::identity::HoldingIdentity hi = net::corda::data::identity::HoldingIdentity();
    hi.x500Name = "Some INVALID X500";
    hi.groupId = "GROUP 1";

    ASSERT_NO_THROW (avro::encode(*e, hi));
}

/**********************************************************************************************************************/

TEST_F (AvroTests, holdingIdLS2) { // NOLINT
    std::string hiSchema = {R"(
        {
            "type": "record",
            "name": "IgnoreMe",
            "namespace": "net.corda.data.identity",
            "fields": [
                {
                    "name": "x500Name",
                    "type": "int"
                }
            ]
        },
        {
            "type": "record",
            "name": "HoldingIdentity",
            "namespace": "net.corda.data.identity",
            "fields": [
                {
                    "name": "x500Name",
                    "type": "string"
                },
                {
                    "name": "groupId",
                    "type": "string"
                }
            ]
        }
    )"
    };

    avro::ValidSchema schema;
    schema = avro::compileJsonSchemaFromString(hiSchema);

    std::cout << schema.toJson(true) << std::endl;

    std::unique_ptr <avro::OutputStream> out2 = avro::memoryOutputStream();

    avro::EncoderPtr e = avro::jsonEncoder (schema);
    e->init (*out2);

    net::corda::data::identity::HoldingIdentity hi = net::corda::data::identity::HoldingIdentity();
    hi.x500Name = "Some INVALID X500";
    hi.groupId = "GROUP 1";

    /**
     * We expect this to f up since the schema doesn't contain the propertypes
     */
    ASSERT_ANY_THROW(avro::encode(*e, hi));
}

/**********************************************************************************************************************/

TEST_F (AvroTests, holdingId) { // NOLINT
    net::corda::data::identity::HoldingIdentity hi = net::corda::data::identity::HoldingIdentity();

    std::unique_ptr <avro::OutputStream> out1 = avro::memoryOutputStream();

    auto l_schema = avro::ValidSchema();
    std::ifstream ifs;
    ifs.open ("avro.schema");
    ifs.seekg(std::ifstream::beg);
    avro::compileJsonSchema (ifs, l_schema);
    avro::EncoderPtr e = avro::jsonEncoder (l_schema);
    e->init (*out1);

    hi.x500Name = "Some INVALID X500";
    hi.groupId = "GROUP 1";

    try {
        avro::encode(*e, hi);
    } catch (const std::exception & excp) {
        std::cout << "ERROR: " << excp.what() << std::endl;
        throw excp;
    }
}

/**********************************************************************************************************************/

TEST_F (AvroTests, unauthMsgHdr) { // NOLINT
    std::string s = {R"((
    {
        "type": "record",
        "name": "HoldingIdentity",
        "namespace": "net.corda.data.identity",
        "fields": [
            {
                "name": "x500Name",
                "type": "string"
            },
            {
                "name": "groupId",
                "type": "string"
            }
        ]
    },
    {
        "type": "record",
        "name": "UnauthenticatedMessageHeader",
        "namespace": "net.corda.p2p.app",
        "fields": [
            {
                "name": "destination",
                "type": "net.corda.data.identity.HoldingIdentity"
            },
            {
                "name": "source",
                "type": "net.corda.data.identity.HoldingIdentity"
            },
            {
                "name": "subsystem",
                "doc": "The subsystem that this message is sent from and should be received by.",
                "type": "string"
            }
        ]
    }))"};

    auto uhm = net::corda::p2p::app::UnauthenticatedMessageHeader();
    uhm.subsystem = "subby";
    uhm.source.x500Name = "source500";
    uhm.source.groupId = "group 1";
    uhm.destination.x500Name = "dest500";
    uhm.destination.groupId = "group 2";

    auto lschema = avro::compileJsonSchemaFromString (s);

    std::unique_ptr <avro::OutputStream> out = avro::memoryOutputStream();

    avro::EncoderPtr e = avro::jsonEncoder (lschema);
    e->init (*out);

    try {
        avro::encode(*e, uhm);
    } catch (const std::exception & excp) {
        std::cout << "ERROR: " << excp.what() << std::endl;
        throw excp;

    }
}

/**********************************************************************************************************************/

TEST_F (AvroTests, unauthMsgHdrFromSchemaFile) { // NOLINT

    auto uhm = net::corda::p2p::app::UnauthenticatedMessageHeader();
    uhm.subsystem = "subby";
    uhm.source.x500Name = "source500";
    uhm.source.groupId = "group 1";
    uhm.destination.x500Name = "dest500";
    uhm.destination.groupId = "group 2";

    std::unique_ptr <avro::OutputStream> out = avro::memoryOutputStream();

    std::ifstream ifs;
    auto lschema = avro::ValidSchema();
    ifs.open ("avro.schema");
    avro::compileJsonSchema (ifs, lschema);

    avro::EncoderPtr e = avro::jsonEncoder (lschema);
    e->init (*out);

    try {
        avro::encode(*e, uhm);
    } catch (const std::exception & excp) {
        std::cout << "ERROR: " << excp.what() << std::endl;
        throw excp;

    }
}

/**********************************************************************************************************************/

TEST_F (AvroTests, BuiltSchema1) {
    auto uhm = net::corda::p2p::app::UnauthenticatedMessageHeader();
    uhm.subsystem = "subby";
    uhm.source.x500Name = "source500";
    uhm.source.groupId = "group 1";
    uhm.destination.x500Name = "dest500";
    uhm.destination.groupId = "group 2";

    std::unique_ptr <avro::OutputStream> out = avro::memoryOutputStream();

    avro::EncoderPtr e = avro::jsonEncoder (avro::ValidSchema (corda::p2p::messaging::buildUnauthenticatedMessageSchema()));
    e->init (*out);

    try {
        avro::encode(*e, uhm);
    } catch (const std::exception & excp) {
        std::cout << "ERROR: " << excp.what() << std::endl;
        throw excp;
    }
}

/**********************************************************************************************************************/
