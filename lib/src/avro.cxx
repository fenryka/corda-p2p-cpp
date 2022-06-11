#include "avro.h"
#include "avro_local.h"

void
test() {
    std::unique_ptr <avro::OutputStream> out = avro::memoryOutputStream();
    avro::EncoderPtr e = avro::binaryEncoder();
    e->init(*out);

    corda_p2p::HoldingIdentity hi;
    hi.x500Name = "poop";
}

