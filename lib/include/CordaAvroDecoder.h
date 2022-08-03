#pragma once

#include <vector>
#include <string>
#include <avro/Stream.hh>
#include <avro/ValidSchema.hh>
#include <avro/Encoder.hh>
#include <avro/Decoder.hh>

#include <iostream>

#include "CordaAvroInteralTypes.h"

/**********************************************************************************************************************/

namespace corda::p2p::messaging {

    class CordaAvroSerializable;

}

/**********************************************************************************************************************/

/**
 * Wrap the AVRO library to allow easy streaming
 */
namespace corda::p2p::avro {

    template<typename T>
    class CordaAvroDecoder {
    private :
        const ::avro::ValidSchema & m_schema;

    public :
        explicit CordaAvroDecoder (const ::avro::ValidSchema & schema_)
            : m_schema { schema_ }
        {

        }

        virtual ~CordaAvroDecoder() = default;

        virtual std::unique_ptr<T> decode() = 0;

        [[nodiscard]] decltype (m_schema) & schema() const { return m_schema; }
    };

    namespace internal {

        template<typename T, class FORMAT> // NOLINT
        class CordaAvroSpecDecoder : public CordaAvroDecoder<T> {
        public :
            explicit CordaAvroSpecDecoder (const ::avro::ValidSchema & schema_)
                : CordaAvroDecoder<T>(schema_)
            { }

            std::unique_ptr<T> decode () override;

            virtual ::avro::InputStreamPtr stream() = 0;
        };
    }
}

/**********************************************************************************************************************/

namespace corda::p2p::avro::internal {

    template<class T, class TYPE>
    class FileDecoder : public internal::CordaAvroSpecDecoder<T, TYPE> {
    private :
        std::string m_fileName;
    public :
        FileDecoder (std::string fileName_, const ::avro::ValidSchema &schema_)
                : internal::CordaAvroSpecDecoder<T, TYPE>(schema_), m_fileName { std::move(fileName_) }
        {
        }

        ~FileDecoder() override = default;

        ::avro::InputStreamPtr stream() override {
            return ::avro::fileInputStream (m_fileName.c_str());
        }
    };

    template<class T, class TYPE>
    class MemDecoder : public internal::CordaAvroSpecDecoder<T, TYPE> {
    public :
        explicit MemDecoder (const ::avro::ValidSchema &schema_)
                : internal::CordaAvroSpecDecoder<T, TYPE>(schema_)
        {
        }

        ~MemDecoder() override = default;

        ::avro::InputStreamPtr stream() override = 0;
    };

    template<class T, class TYPE>
    class MemDecoderStream : public MemDecoder<T, TYPE> {
    private :
        ::avro::OutputStreamPtr m_stream;
    public :
        explicit MemDecoderStream (const ::avro::ValidSchema &schema_, ::avro::OutputStreamPtr && stream_)
                : internal::MemDecoder<T, TYPE>(schema_)
                , m_stream (std::move (stream_))
        {
        }

        ~MemDecoderStream() override = default;

        ::avro::InputStreamPtr stream() override {
            return ::avro::memoryInputStream (*m_stream);
        }
    };

    template<class T, class TYPE>
    class MemDecoderBlob : public internal::MemDecoder<T, TYPE> {
    private :
        const uint8_t * m_data;
        size_t m_len;
    public :
        explicit MemDecoderBlob (const ::avro::ValidSchema &schema_, const uint8_t * data_, size_t len_)
                : internal::MemDecoder<T, TYPE>(schema_)
                , m_data { data_ }
                , m_len { len_ }
        {
        }

        ~MemDecoderBlob() override = default;

        ::avro::InputStreamPtr stream() override {
            return ::avro::memoryInputStream (m_data, m_len);
        }
    };
}

/**********************************************************************************************************************/

template<class T, class FORMAT>
std::unique_ptr<T>
corda::p2p::avro::internal::
CordaAvroSpecDecoder<T, FORMAT>::decode () {
    auto in = stream();
    auto decoder = internal::XCoder<FORMAT>::decoder (this->schema());
    decoder->init (*in);

    try {
        return T::decode (decoder);
    } catch (const std::exception & excp) {
        std::cerr << __FILE__ << "::" << __LINE__ << " - " << excp.what() << std::endl;
        throw excp;
    }
}

/**********************************************************************************************************************/

