#include <gtest/gtest.h>
#include <iostream>
#include "../src/avro.h"
#include "X500Support.h"

#include "avro/Encoder.hh"
#include "avro/Decoder.hh"


/*
 * Stupid little test to just make sure Alice is who we need her to be
 */
TEST (avro, alice) { // NOLINT
    corda::p2p::identity::test::Alice alice;
    ASSERT_EQ (alice.name(), "O=Alice, L=London, C=London");
}


TEST (avro, test1) { // NOLINT
    corda::p2p::identity::test::Alice alice;
    corda_p2p::HoldingIdentity hi;

    std::unique_ptr <avro::OutputStream> out = avro::memoryOutputStream();

    avro::EncoderPtr e = avro::binaryEncoder();
    e->init (*out);

    hi.x500Name = alice.name();
    avro::encode(*e, hi);

    std::unique_ptr<avro::InputStream> in = avro::memoryInputStream(*out);
    avro::DecoderPtr d = avro::binaryDecoder();
    d->init(*in);


    corda_p2p::HoldingIdentity hi2 ;
    avro::decode(*d, hi2);

    std::cout << hi2.x500Name << std::endl;
}

TEST (avro, unauthMsg) { // NOLINT
    corda_p2p::AppMessage am;
    corda_p2p::UnauthenticatedMessage um;
    corda_p2p::UnauthenticatedMessageHeader umh;
    corda_p2p::HoldingIdentity from;
    corda_p2p::HoldingIdentity to;

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



//    am.message
}
