#include <gtest/gtest.h>

#include "CordaAvroXCoderFactory.h"
#include "CordaAvroSerializable.h"

#include "X500Support.h"

#include "../src/avro-manual.h"
#include "../src/SchemaBuilder.h"

template<class T> using factory_t = corda::p2p::avro::CordaAvroFactory<T>;
using AppMessage = corda::p2p::messaging::AppMessage;
using Envelope = corda::p2p::messaging::Envelope;

/*
 * Just make sure we can, using raw AVRO, actually round-trip an AppMessage
 */
TEST (RTT, test0) { // NOLINT
    net::corda::data::identity::HoldingIdentity from, to;

    from.x500Name = corda::p2p::identity::test::AliceX500().name();
    from.groupId = "group-1";
    to.x500Name = corda::p2p::identity::test::Bob().name();
    to.groupId = "group-1";

    net::corda::p2p::app::UnauthenticatedMessageHeader unAuthMessageHeader;
    unAuthMessageHeader.destination = to;
    unAuthMessageHeader.source = from;
    unAuthMessageHeader.subsystem = "subsystem";

    net::corda::p2p::app::UnauthenticatedMessage unAuthMessage;
    unAuthMessage.header = unAuthMessageHeader;
    unAuthMessage.payload = std::vector<uint8_t> { 'h', 'e', 'l', 'l', 'o' };

    net::corda::p2p::app::AppMessage am;
    am.message.set_UnauthenticatedMessage(unAuthMessage);


    auto out = ::avro::memoryOutputStream();
    auto encoder = ::avro::validatingEncoder (
            ::avro::ValidSchema (::corda::p2p::messaging::buildAppMessageSchema()),
            ::avro::binaryEncoder());
    encoder->init (*out);

    ::avro::encode (*encoder, am);
    out->flush();
    std::cout << "ByteCount = " << out->byteCount();

    auto in = ::avro::memoryInputStream(*out);

    auto decoder = ::avro::validatingDecoder(
            ::avro::ValidSchema (::corda::p2p::messaging::buildAppMessageSchema()),
            ::avro::binaryDecoder());

    decoder->init (*in);

    net::corda::p2p::app::AppMessage decodedMessage;
    ::avro::decode (*decoder, decodedMessage);
}


TEST (RTT, test1) { // NOLINT
    factory_t<AppMessage> appFactory;
    auto encoder = appFactory.encodeAsBinaryToMem();

    auto bytes = std::vector<uint8_t> { 'h', 'e', 'l', 'l', 'o' };
    AppMessage am (bytes, corda::p2p::identity::test::Alice::ptr(), corda::p2p::identity::test::Bob::ptr());

    auto encodedMessage = encoder->encode(am);
    std::cout << "ByteCount = " << encodedMessage->byteCount() << std::endl;

    auto decoder = appFactory.decodeAsBinaryFromMem (std::move (encodedMessage));

    auto decodedMessage = decoder->decode();
    std::string decodedString {decodedMessage->payload().begin(), decodedMessage->payload().end()};
    std::cout << decodedString << std::endl;
}

TEST (RTT, test2) { //NOLINT
    factory_t<AppMessage> appFactory;
    auto msgEncoder = appFactory.encodeAsBinaryToMem();
    auto bytes = std::vector<uint8_t> { 'h', 'e', 'l', 'l', 'o' };
    AppMessage am (bytes, corda::p2p::identity::test::Alice::ptr(), corda::p2p::identity::test::Bob::ptr());
    auto encodedMessage = msgEncoder->encode(am);
    auto messageBytes = ::avro::snapshot(*encodedMessage);

    /*
     * With the message built, wrap it in the nevelope
     */
    factory_t<Envelope> envelopeFactory;
    auto envEncoder = envelopeFactory.encodeAsBinaryToMem();
    Envelope envelope (99, am.fingerprint(), messageBytes);

    auto encodedEnvelope = envEncoder->encode (envelope);

    /*
     * And now lets decode the envelope
     */
    auto envelopeDecoder = envelopeFactory.decodeAsBinaryFromMem (std::move (encodedEnvelope));
    auto decodedEnvelope = envelopeDecoder->decode();

    /*
     *
     */
    auto appDecoder = appFactory.decodeAsBinaryFromMem (
            decodedEnvelope->payload()->data(),
            decodedEnvelope->payload()->size());

    auto decodedAppMessage = appDecoder->decode();
    ASSERT_EQ( "hello",
               std::string (
                       decodedAppMessage->payload().begin(),
                       decodedAppMessage->payload().end()));
}
