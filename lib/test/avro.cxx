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
