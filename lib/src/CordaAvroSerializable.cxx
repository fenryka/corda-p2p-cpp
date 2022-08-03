#include <utility>

#include <avro/Schema.hh>

#include "CordaAvroSerializable.h"
#include "Identity.h"
#include "CordaAvroEncoder.h"

/*
 * Remember once generator is better we can not use the manual version
 */
#include "avro-manual.h"

#include "envelope.h"
#include "SchemaBuilder.h"
#include "Deflator.h"

/**********************************************************************************************************************/

std::array<uint8_t, 32> corda::p2p::messaging::Envelope::s_fingerprint = {
        0,  211, 69,  23,  209, 234, 134, 149, 154, 162, 252, 41,  171, 151, 217, 131,
        73, 179, 123, 141, 149, 57,  177, 77,  144, 183, 203, 141, 41,  155, 240, 55
};

/**********************************************************************************************************************/

corda::p2p::messaging::
Envelope::Envelope()
        : CordaAvroSerializable()
        , m_magic { 'c', 'o', 'r', 'd', 'a', 1, 0, 0 }
        , m_flags { 1 & 0x1 }
        , m_fingerprint { }
        , m_payload { nullptr }
{

}
/**********************************************************************************************************************/

/*
 *
 */
corda::p2p::messaging::
Envelope::Envelope (int flags_, std::array<uint8_t, 32> fingerprint_, std::shared_ptr<std::vector<uint8_t>> payload_)
        : CordaAvroSerializable()
        , m_magic { 'c', 'o', 'r', 'd', 'a', 1, 0, 0 }
        , m_flags { flags_ }
        , m_fingerprint { fingerprint_ }
        , m_payload { std::move (payload_) }
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
    envelope.magic = m_magic;
    envelope.fingerprint = m_fingerprint;

    ::avro::encode (*e_, envelope);
}

/**********************************************************************************************************************/

std::unique_ptr<corda::p2p::messaging::Envelope>
corda::p2p::messaging::
Envelope::decode (::avro::DecoderPtr d_) {
    auto rtn = std::unique_ptr<Envelope>(new Envelope());

    net::corda::data::AvroEnvelope envelope;
    ::avro::decode(*d_, envelope);

    rtn->m_payload = std::make_shared<std::vector<uint8_t>> (std::move (envelope.payload));
    rtn->m_fingerprint = envelope.fingerprint;
    rtn->m_flags = envelope.flags;

    return rtn;
}

/**********************************************************************************************************************/

::avro::Schema
corda::p2p::messaging::
Envelope::schema()  {
    return buildEnvelopeSchema();
}

std::vector<uint8_t>
corda::p2p::messaging::
Envelope::decompress() {
    return inflate (*m_payload);
}

/**********************************************************************************************************************
 *
 * class corda::p2p::messaging::AppMessage
 *
 **********************************************************************************************************************/

std::map<std::string, std::array<uint8_t, 32>> corda::p2p::messaging::AppMessage::s_fingerprints { // NOLINT
        {
                "p2p-preview",
                {
                        239,  93,  87, 105,  38,  94, 144, 82, 5, 150,  21, 202, 87, 71, 140, 245,
                        180, 141, 177, 196, 161, 225,  88,  0, 3, 158, 210,  62, 86, 89, 232, 158
                }
        },
        {
                "head",
                {
                        182, 179, 207,  37,  62,  52, 240, 136, 133,  27, 241, 223, 130, 195, 183, 237,
                        155, 179, 140, 194, 230, 123, 255, 243, 149, 148, 240,  47, 147,  29,  28, 215
                }
        }

};

/**********************************************************************************************************************/

const std::array<uint8_t, 32> &
corda::p2p::messaging::
AppMessage::fingerprint() {
    return s_fingerprints["p2p-preview"];
}

/**********************************************************************************************************************/

::avro::Schema
corda::p2p::messaging::
AppMessage::schema() {
    return buildAppMessageSchema();
}

/**********************************************************************************************************************/

corda::p2p::messaging::
AppMessage::AppMessage ()
    : m_payload { }
    , m_source { nullptr }
    , m_destination {nullptr }
{ }

/**********************************************************************************************************************/

corda::p2p::messaging::
AppMessage::AppMessage (
        std::vector<uint8_t> payload_,
        std::shared_ptr<identity::Identity> source_,
        std::shared_ptr<identity::Identity> dest_)
            : m_payload { std::move (payload_) }
            , m_source { std::move (source_) }
            , m_destination { std::move (dest_) }
{
}

/**********************************************************************************************************************/

void
corda::p2p::messaging::
AppMessage::encode (::avro::EncoderPtr e_) const {
    net::corda::data::identity::HoldingIdentity from, to;

    from.x500Name = m_source->name();
    from.groupId = "group-1";
    to.x500Name = m_destination->name();
    to.groupId = "group-1";

    net::corda::p2p::app::UnauthenticatedMessageHeader unAuthMessageHeader;
    unAuthMessageHeader.destination = to;
    unAuthMessageHeader.source = from;
    unAuthMessageHeader.subsystem = "subsystem";

    net::corda::p2p::app::UnauthenticatedMessage unAuthMessage;
    unAuthMessage.header = unAuthMessageHeader;
    unAuthMessage.payload = std::vector<uint8_t> { m_payload.begin(), m_payload.end() };

    net::corda::p2p::app::AppMessage am;
    am.message.set_UnauthenticatedMessage(unAuthMessage);

    ::avro::encode (*e_, am);
}

/**********************************************************************************************************************/

std::unique_ptr<corda::p2p::messaging::AppMessage>
corda::p2p::messaging::
AppMessage::decode (const ::avro::DecoderPtr& d_) {
    auto rtn = std::unique_ptr<AppMessage>(new AppMessage());

    net::corda::p2p::app::AppMessage am;

    try {
        ::avro::decode (*d_, am);
    } catch (const std::exception & e) {
        std::cerr << e.what() << std::endl;
        throw e;
    }

    const auto & um = am.message.get_UnauthenticatedMessage();
    rtn->m_source = std::make_shared<identity::Identity> (um.header.source.x500Name);
    rtn->m_destination = std::make_shared<identity::Identity> (um.header.destination.x500Name);
    rtn->m_payload = std::vector<uint8_t> (um.payload);

    return rtn;
}

/**********************************************************************************************************************/

std::vector<uint8_t>
corda::p2p::messaging::
AppMessage::decompressedPayload() {
    return inflate (m_payload);
}
