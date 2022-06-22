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
Message::Message (
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

std::unique_ptr <avro::OutputStream>
corda::p2p::messaging::
Message::encode(const corda::p2p::identity::Identity & dest_) {
    std::unique_ptr <avro::OutputStream> out = avro::memoryOutputStream();

    avro::EncoderPtr e = avro::jsonEncoder (m_schema);
    e->init (*out);

    corda_p2p::AppMessage am;
    corda_p2p::UnauthenticatedMessage unAuthMessage;
    corda_p2p::UnauthenticatedMessageHeader unAuthMessageHeader;
    corda_p2p::HoldingIdentity from;
    corda_p2p::HoldingIdentity to;

    from.x500Name = m_source.name();
    from.groupId = "group1";
    to.x500Name = dest_.name();
    to.groupId = "group1";

    unAuthMessageHeader.destination = to;
    unAuthMessageHeader.source = from;
    unAuthMessageHeader.subsystem = "subsystem";

    unAuthMessage.header = unAuthMessageHeader;
    unAuthMessage.payload = m_payload;

    am.message.set_UnauthenticatedMessage(unAuthMessage);

    try {
        avro::encode(*e, am);
    } catch (const std::exception & e) {
        std::cout << "ERROR: " << e.what() << std::endl;
        throw e;
    }

    return out;
}

/**********************************************************************************************************************/
