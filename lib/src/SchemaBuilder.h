#pragma once


#include <avro/ValidSchema.hh>

namespace corda::p2p::messaging {

    avro::Schema buildHoldingIdentitySchema();

    avro::Schema buildUnauthenticatedMessageHeaderSchema();

    avro::Schema buildAuthenticatedMessageHeaderSchema();

    avro::Schema buildUnauthenticatedMessageSchema();

    avro::Schema buildAuthenticatedMessageSchema();

    avro::Schema buildAppMessageSchema();

}