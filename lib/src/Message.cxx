#include <utility>

#include "Message.h"
#include "Identity.h"

#include "avro/Encoder.hh"

/*
 * Our Auto Generated header based on the AVRO schema
 */
#include "avro.h"

/**********************************************************************************************************************/

corda::p2p::messaging::
BaseMessage::BaseMessage (
        const avro::ValidSchema & schema_,
        std::string  key_,
        const std::string & payload_,
        const corda::p2p::identity::Identity & source_)
    : m_schema { schema_ }
    , m_key (std::move(key_))
    , m_payload { payload_.begin(), payload_.end() }
    , m_source { source_ }
{
//    std::cout << schema_.toJson(true) << std::endl;
}

/**********************************************************************************************************************/

corda_p2p::AppMessage
buildAppMessage (
        const corda::p2p::messaging::BaseMessage & message_,
        const corda::p2p::identity::Identity & to_
) {
    corda_p2p::AppMessage am;
    corda_p2p::UnauthenticatedMessage unAuthMessage;
    corda_p2p::UnauthenticatedMessageHeader unAuthMessageHeader;
    corda_p2p::HoldingIdentity from;
    corda_p2p::HoldingIdentity to;

    from.x500Name = message_.source().name();
    from.groupId = "1";
    to.x500Name = to_.name();
    to.groupId = "1";

    unAuthMessageHeader.destination = to;
    unAuthMessageHeader.source = from;
    unAuthMessageHeader.subsystem = "subsystem";

    unAuthMessage.header = unAuthMessageHeader;
    unAuthMessage.payload = message_.payload();

    am.message.set_UnauthenticatedMessage(unAuthMessage);

    return am;
}

/**********************************************************************************************************************/

std::unique_ptr<avro::OutputStream>
corda::p2p::messaging::
MemEncoder::encode_ ( // NOLINT
        const corda::p2p::messaging::BaseMessage & message_,
        const identity::Identity & to_
) {
    auto am = buildAppMessage(message_, to_);

    std::unique_ptr <avro::OutputStream> out = avro::memoryOutputStream();

    avro::EncoderPtr e = avro::jsonEncoder (message_.schema());
    e->init (*out);

    try {
        avro::encode(*e, am);
    } catch (const std::exception & e) {
        std::cout << "ERROR: " << e.what() << std::endl;
        throw e;
    }

    return out;
}

/**********************************************************************************************************************/
std::unique_ptr<avro::OutputStream>
corda::p2p::messaging::
FileEncoder::encode_ ( // NOLINT
        const corda::p2p::messaging::BaseMessage & message_,
        const identity::Identity & to_
) {
    auto am = buildAppMessage(message_, to_);

    std::unique_ptr <avro::OutputStream> out = avro::fileOutputStream(m_fileName.c_str());

    avro::EncoderPtr e = avro::jsonEncoder (message_.schema());
    e->init (*out);

    try {
        avro::encode(*e, am);
    } catch (const std::exception & e) {
        std::cout << "ERROR: " << e.what() << std::endl;
        throw e;
    }

    return out;
}

/**********************************************************************************************************************/
