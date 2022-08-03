#pragma once

#include <avro/Encoder.hh>
#include <avro/Decoder.hh>

/**********************************************************************************************************************/

namespace corda::p2p::avro::internal {

    struct Binary {};
    struct Json {};

    template<typename T>
    struct XCoder {};

    template<>
    struct XCoder<Binary> {
        [[maybe_unused]] static ::avro::EncoderPtr encoder(const ::avro::ValidSchema &schema_) {
            return ::avro::validatingEncoder (schema_, ::avro::binaryEncoder());
        }

        [[maybe_unused]] static ::avro::DecoderPtr decoder(const ::avro::ValidSchema &schema_) {
            //return ::avro::validatingDecoder (schema_, ::avro::binaryDecoder());
            return ::avro::binaryDecoder();
        }
    };

    template<>
    struct XCoder<Json> {
        [[maybe_unused]] static ::avro::EncoderPtr encoder(const ::avro::ValidSchema &schema_) {
            return ::avro::jsonEncoder (schema_);
        }

        [[maybe_unused]] static ::avro::DecoderPtr decoder(const ::avro::ValidSchema &schema_) {
            return ::avro::jsonDecoder (schema_);
        }
    };
}

/**********************************************************************************************************************/
