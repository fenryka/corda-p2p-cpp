#include <utility>

#include "Message.h"
#include "Identity.h"

#include "avro/Encoder.hh"
#include "avro.h"

/**********************************************************************************************************************/

corda::p2p::messaging::
Message::Message (
        std::string  key_,
        const std::string & payload_,
        const corda::p2p::identity::Identity & source_)
    : m_key(std::move(key_))
    , m_payload { payload_.begin(), payload_.end() }
    , m_source { source_ }
{

}

/**********************************************************************************************************************/

std::unique_ptr <avro::OutputStream>
corda::p2p::messaging::
Message::encode(const corda::p2p::identity::Identity & dest_) {
    std::unique_ptr <avro::OutputStream> out = avro::memoryOutputStream();

    avro::EncoderPtr e = avro::binaryEncoder();
    e->init (*out);

    corda_p2p::AppMessage am;
    corda_p2p::UnauthenticatedMessage unAuthMessage;
    corda_p2p::UnauthenticatedMessageHeader unAuthMessageHeader;
    corda_p2p::HoldingIdentity from;
    corda_p2p::HoldingIdentity to;

    from.x500Name = m_source.name();
    to.x500Name = dest_.name();

    unAuthMessageHeader.destination = to;
    unAuthMessageHeader.source = from;

    unAuthMessage.header = unAuthMessageHeader;
    unAuthMessage.payload = m_payload;

    am.message.set_UnauthenticatedMessage(unAuthMessage);

    avro::encode(*e, am);

    return out;
}

/**********************************************************************************************************************/
