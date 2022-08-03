#pragma once

#include <avro/ValidSchema.hh>

#include "CordaAvroEncoder.h"
#include "CordaAvroDecoder.h"

namespace corda::p2p::avro {

    template<class T>
    class CordaAvroFactory {
    private :
        ::avro::ValidSchema m_schema;
    public :

        explicit CordaAvroFactory()
                : m_schema { ::avro::ValidSchema { T::schema() } }
        {
        }

        std::unique_ptr<corda::p2p::avro::CordaAvroEncoder<T>>
        encodeAsJSONToFile(const std::string & fileName_) {
            return std::make_unique<internal::FileEncoder<T, internal::Json>> (
                    internal::FileEncoder<T, internal::Json> { fileName_, m_schema } );
        }

        std::unique_ptr<corda::p2p::avro::CordaAvroEncoder<T>>
        encodeAsJSONToMem() {
            return std::make_unique<internal::MemEncoder<T, internal::Json>> (
                    internal::MemEncoder<T, internal::Json> { m_schema } );
        }

        std::unique_ptr<corda::p2p::avro::CordaAvroEncoder<T>>
        encodeAsBinaryToFile (const std::string & fileName_) {
            return std::make_unique<internal::FileEncoder<T, internal::Binary>> (
                    internal::FileEncoder<T, internal::Binary> { fileName_, m_schema } );
        }


        std::unique_ptr<corda::p2p::avro::CordaAvroEncoder<T>>
        encodeAsBinaryToMem() {
            return std::make_unique<internal::MemEncoder<T, internal::Binary>> (
                    internal::MemEncoder<T, internal::Binary> { m_schema } );
        }

        std::unique_ptr<corda::p2p::avro::CordaAvroDecoder<T>>
        decodeAsJSONFromFile (const std::string & fileName_) {
            return std::make_unique<internal::FileDecoder<T, internal::Json>> (
                    internal::FileDecoder<T, internal::Json> { fileName_, m_schema } );
        }

        std::unique_ptr<corda::p2p::avro::CordaAvroDecoder<T>>
        decodeAsJSONFromMem (
                const uint8_t * data_,
                size_t len_
        ) {
            return std::make_unique<internal::MemDecoder<T, internal::Json>> (
                    internal::MemDecoder<T, internal::Json> { m_schema, data_, len_ } );
        }

        std::unique_ptr<corda::p2p::avro::CordaAvroDecoder<T>>
        decodeAsBinaryFromFile (const std::string & fileName_) {
            return std::make_unique<internal::FileDecoder<T, internal::Binary>> (
                    internal::FileDecoder<T, internal::Binary> { fileName_, m_schema } );
        }

        std::unique_ptr<corda::p2p::avro::CordaAvroDecoder<T>>
        decodeAsBinaryFromMem(
                const uint8_t * data_,
                size_t len_
        ) {
            return std::make_unique<internal::MemDecoderBlob<T, internal::Binary>> (
                    internal::MemDecoderBlob<T, internal::Binary> { m_schema, data_, len_ } );
        }

        std::unique_ptr<corda::p2p::avro::CordaAvroDecoder<T>>
        decodeAsBinaryFromMem(::avro::OutputStreamPtr && stream_) {
            return std::unique_ptr<internal::MemDecoderStream<T, internal::Binary>> (new
                    internal::MemDecoderStream<T, internal::Binary> { m_schema, std::move (stream_) } );
        }
    };

}
