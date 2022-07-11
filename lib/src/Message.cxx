#include <utility>

#include <avro/Schema.hh>

#include "Message.h"
#include "Identity.h"
#include "CordaAvro.h"

/*
 * Remember once generator is better we can not use the manual version
 */
#include "avro-manual.h"

#include "envelope.h"
#include "SchemaBuilder.h"
#include "debug.h"

/**********************************************************************************************************************/

corda::p2p::messaging::
Message::Message()
    : m_magic { 'c', 'o', 'r', 'd', 'a', 1, 0, 0 }
{}

/**********************************************************************************************************************/

std::array<uint8_t, 32> corda::p2p::messaging::Envelope::s_fingerprint = {
        0, 211, 69, 23, 209, 234, 134, 149, 154, 162, 252, 41, 171, 151, 217, 131, 73,
        179, 123, 141,149, 57, 177, 77, 144, 183, 203, 141, 41, 155, 240, 55
};

/**********************************************************************************************************************/

/*
 *
 */
corda::p2p::messaging::
Envelope::Envelope (int flags_, std::array<uint8_t, 32> fingerprint_, std::shared_ptr<std::vector<uint8_t>> payload_)
        : Message()
        , m_flags { flags_ }
        , m_fingerprint { fingerprint_ }
        , m_payload { std::move ( payload_ ) }
{ }

/**********************************************************************************************************************/

const std::array<uint8_t, 32> &
corda::p2p::messaging::
Envelope::fingerprint() {
    return s_fingerprint;
}

/**********************************************************************************************************************/

void
corda::p2p::messaging::
Envelope::encode (::avro::EncoderPtr e_) const {
    net::corda::data::AvroEnvelope envelope;

    envelope.payload = std::vector<uint8_t> { m_payload->begin(), m_payload->end() };
    envelope.flags = m_flags;
    envelope.magic = magic();
    envelope.fingerprint = m_fingerprint;

    ::avro::encode (*e_, envelope);
}

/**********************************************************************************************************************/

std::unique_ptr<corda::p2p::avro::CordaAvroFactory>
corda::p2p::messaging::
Envelope::factory() {
    return std::make_unique<corda::p2p::avro::CordaAvroFactory> (
            ::avro::ValidSchema (corda::p2p::messaging::buildEnvelopeSchema()));
}

/**********************************************************************************************************************
 *
 * class corda::p2p::messaging::AppMessage
 *
 **********************************************************************************************************************/

/*
 * AppMessage.avsc::[B@4f209819
{ 182, 179, 207, 37, 62, 52, 240, 136, 133, 27, 241, 223, 130, 195, 183, 237,
  155, 179, 140, 194, 230, 123, 255, 243, 149, 148, 240, 47, 147, 29, 28, 215 }
{ -74, -77, -49, 37, 62, 52, -16, -120, -123, 27, -15, -33, -126, -61, -73, -19,
  -101, -77, -116, -62, -26, 123, -1, -13, -107, -108, -16, 47, -109, 29, 28, -41 }
 */

#if 0
std::array<uint8_t, 32> corda::p2p::messaging::AppMessage::s_fingerprint = {
        182, 179, 207, 37, 62, 52, 240, 136, 133, 27, 241, 223, 130, 195, 183, 237,
        155, 179, 140, 194, 230, 123, 255, 243, 149, 148, 240, 47, 147, 29, 28, 215 };
#else
std::array<uint8_t, 32> corda::p2p::messaging::AppMessage::s_fingerprint = {
        static_cast<unsigned char>(-17),   93,  87,  105, 38, 94, static_cast<unsigned char>(-112), 82,   5, static_cast<unsigned char>(-106), 21, static_cast<unsigned char>(-54), 87, 71, static_cast<unsigned char>(-116), static_cast<unsigned char>(-11), static_cast<unsigned char>(-76),
        static_cast<unsigned char>(-115), static_cast<unsigned char>(-79), static_cast<unsigned char>(-60), static_cast<unsigned char>(-95), static_cast<unsigned char>(-31), 88,    0,  3, static_cast<unsigned char>(-98),  static_cast<unsigned char>(-46), 62, 86, 89, static_cast<unsigned char>(-24), static_cast<unsigned char>(-98) };
#endif
/**********************************************************************************************************************/

const std::array<uint8_t, 32> &
corda::p2p::messaging::
AppMessage::fingerprint() {
    return s_fingerprint;
}

/**********************************************************************************************************************/

corda::p2p::messaging::
AppMessage::AppMessage (
        std::vector<uint8_t> payload_,
        const identity::Identity & source_,
        const identity::Identity & dest_)
            : m_payload { std::move (payload_) }
            , m_source { source_ }
            , m_dest { dest_ }
{
    DBG ("Construct AppMessage [" << source_.name() << "] -> [" << dest_.name() << "]" << std::endl);
}

/**********************************************************************************************************************/

void
corda::p2p::messaging::
AppMessage::encode (::avro::EncoderPtr e_) const {
    DBG (__FUNCTION__ << "::" << std::endl);
    net::corda::data::identity::HoldingIdentity from, to;

    from.x500Name = m_source.name();
    from.groupId = "group-1";
    to.x500Name = m_dest.name();
    to.groupId = "group-1";

    net::corda::p2p::app::UnauthenticatedMessageHeader unAuthMessageHeader;
    unAuthMessageHeader.destination = to;
    unAuthMessageHeader.source = from;
    unAuthMessageHeader.subsystem = "subsystem";

    net::corda::p2p::app::UnauthenticatedMessage unAuthMessage;
    unAuthMessage.header = unAuthMessageHeader;
    unAuthMessage.payload = m_payload;

    net::corda::p2p::app::AppMessage am;
    am.message.set_UnauthenticatedMessage(unAuthMessage);

    ::avro::encode (*e_, am);

    DBG (__FUNCTION__ << "::done" << std::endl);
}

/**********************************************************************************************************************/

std::unique_ptr<corda::p2p::avro::CordaAvroFactory>
corda::p2p::messaging::
AppMessage::factory() {
    return std::make_unique<corda::p2p::avro::CordaAvroFactory> (
            ::avro::ValidSchema (corda::p2p::messaging::buildAppMessageSchema()));
}

/**********************************************************************************************************************/

