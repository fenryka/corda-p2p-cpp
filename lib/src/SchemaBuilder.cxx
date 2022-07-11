#include "SchemaBuilder.h"

#include <avro/Schema.hh>

/**********************************************************************************************************************/

avro::Schema
corda::p2p::messaging::
buildHoldingIdentitySchema() {
    /**
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
     */

    //auto HoldingIdentity = avro::RecordSchema("HoldingIdentity");
    auto HoldingIdentity = avro::RecordSchema("net.corda.data.identity.HoldingIdentity");
    HoldingIdentity.addField("x500name", avro::StringSchema());
    HoldingIdentity.addField("groupId", avro::StringSchema());

    return HoldingIdentity;
}

/**********************************************************************************************************************/

avro::Schema
corda::p2p::messaging::
buildUnauthenticatedMessageHeaderSchema() {
    /**
        {
            "type": "record",
            "name": "UnauthenticatedMessageHeader",
            "namespace": "net.corda.p2p.app",
            "fields": [
                {
                    "name": "destination", "type": "net.corda.data.identity.HoldingIdentity"
                },
                {
                    "name": "source", "type": "net.corda.data.identity.HoldingIdentity"
                },
                {
                    "name": "subsystem", "type": "string"
                }
            ]
        }
     */

    auto HoldingIdentity = buildHoldingIdentitySchema();
    auto UnauthenticatedMessageHeader = avro::RecordSchema("net.corda.p2p.app.UnauthenticatedMessageHeader");
    UnauthenticatedMessageHeader.addField("destination", HoldingIdentity);
    UnauthenticatedMessageHeader.addField("source", HoldingIdentity);
    UnauthenticatedMessageHeader.addField("subsystem", avro::StringSchema());

    return UnauthenticatedMessageHeader;
}

/**********************************************************************************************************************/

avro::Schema
corda::p2p::messaging::
buildAuthenticatedMessageHeaderSchema() {
    /**
        {
            "type": "record",
            "name": "AuthenticatedMessageHeader",
            "namespace": "net.corda.p2p.app",
            "fields": [
                { "name": "destination", "type": "net.corda.data.identity.HoldingIdentity" },
                { "name": "source", "type": "net.corda.data.identity.HoldingIdentity" },
                { "name": "ttl", "type": ["null", "long"] },
                { "name": "messageId", "type": "string" },
                { "name": "traceId", "type": "string" },
                { "name": "subsystem", "type": "string" }
            ]
        }
     */

    auto HoldingIdentity = buildHoldingIdentitySchema();
    auto AuthenticatedMessageHeader = avro::RecordSchema ("net.corda.p2p.app.AuthenticatedMessageHeader");
    AuthenticatedMessageHeader.addField ("destination", HoldingIdentity);
    AuthenticatedMessageHeader.addField ("source", HoldingIdentity);
    auto ttl = avro::UnionSchema();
    ttl.addType(avro::NullSchema());
    ttl.addType(avro::LongSchema());
    AuthenticatedMessageHeader.addField("ttl", ttl);
    AuthenticatedMessageHeader.addField("messageId", avro::StringSchema());
    AuthenticatedMessageHeader.addField("traceId", avro::StringSchema());
    AuthenticatedMessageHeader.addField("subsystem", avro::StringSchema());

    return AuthenticatedMessageHeader;
}

/**********************************************************************************************************************/

avro::Schema
corda::p2p::messaging::
buildUnauthenticatedMessageSchema() {
    /*
        {
            "type": "record",
            "name": "UnauthenticatedMessage",
            "namespace": "net.corda.p2p.app",
            "fields": [
                { "name": "header", "type": "net.corda.p2p.app.UnauthenticatedMessageHeader" },
                { "name": "payload", "type": "bytes" }
            ]
        }
     */

    auto UnauthenticatedMessage = avro::RecordSchema("net.corda.p2p.app.UnauthenticatedMessage");
    UnauthenticatedMessage.addField("header", buildUnauthenticatedMessageHeaderSchema());
    UnauthenticatedMessage.addField("payload", avro::BytesSchema());

    return UnauthenticatedMessage;
}

/**********************************************************************************************************************/

avro::Schema
corda::p2p::messaging::
buildAuthenticatedMessageSchema() {
    /*
        {
            "type": "record",
            "name": "AuthenticatedMessage",
            "namespace": "net.corda.p2p.app",
            "fields": [
                    { "name": "header", "type": "net.corda.p2p.app.AuthenticatedMessageHeader" },
                    { "name": "payload", "type": "bytes" }
            ]
        }
    */



    auto AuthenticatedMessage = avro::RecordSchema ("net.corda.p2p.app.AuthenticatedMessage");
    AuthenticatedMessage.addField("header", buildAuthenticatedMessageHeaderSchema());
    AuthenticatedMessage.addField("payload", avro::BytesSchema());

    return AuthenticatedMessage;
}

/**********************************************************************************************************************/

avro::Schema
corda::p2p::messaging::
buildAppMessageSchema() {
    /*
        {
            "type": "record",
            "name": "AppMessage",
            "namespace": "net.corda.p2p.app",
            "doc": "An application message to be transferred via the p2p layer.",
            "fields" [
                {
                    "name": "message",
                    "type": [
                        "net.corda.p2p.app.AuthenticatedMessage",
                        "net.corda.p2p.app.UnauthenticatedMessage"
                    ]
                }
            ]
        }
    */

    auto AppMessage = avro::RecordSchema ("net.corda.p2p.app.AppMessage");
    auto message = avro::UnionSchema();
    message.addType(buildAuthenticatedMessageSchema());
    message.addType(buildUnauthenticatedMessageSchema());
    AppMessage.addField("message", message);

    return AppMessage;
}

/**********************************************************************************************************************/

avro::Schema
corda::p2p::messaging::
buildEnvelopeSchema() {
    /*
        {
            "type": "record",
            "name": "AvroEnvelope",
            "namespace": "net.corda.data",
            "fields": [
                {
                    "name": "magic",
                    "type": { "type": "fixed", "size": 8, "name": "Magic" }
                },
                {
                    "name": "fingerprint",
                    "type": { "type": "fixed", "size": 32, "name": "Fingerprint" }
                },
                {
                    "name": "flags",
                    "type": "int"
                },
                {
                    "name": "payload",
                    "type": "bytes"
                }
            ]
        }
    */
    auto envelope = avro::RecordSchema ("net.corda.data.AvroEnvelope");
    envelope.addField("magic", ::avro::FixedSchema(8, "Magic"));
    envelope.addField("fingerprint", ::avro::FixedSchema (32, "Fingerprint"));
    envelope.addField("flags", ::avro::IntSchema());
    envelope.addField("payload", ::avro::BytesSchema());

    return envelope;
}

/**********************************************************************************************************************/
