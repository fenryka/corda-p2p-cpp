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
    class CordaAvroEncoder {
    private :
        const ::avro::ValidSchema & m_schema;

    public :
        explicit CordaAvroEncoder (const ::avro::ValidSchema & schema_)
            : m_schema { schema_ }
        {

        }

        virtual ~CordaAvroEncoder() = default;

        virtual ::avro::OutputStreamPtr encode (const T &) = 0;

        [[nodiscard]] decltype (m_schema) & schema() const { return m_schema; }
    };

    namespace internal {

        template<typename T, class FORMAT>
        class CordaAvroSpec : public CordaAvroEncoder<T> {
        public :
            explicit CordaAvroSpec (const ::avro::ValidSchema & schema_)
                : CordaAvroEncoder<T>(schema_)
            { }

            ::avro::OutputStreamPtr encode (const T &) override;

            virtual ::avro::OutputStreamPtr stream() = 0;
        };
    }
}

/**********************************************************************************************************************/


namespace corda::p2p::avro::internal {

    template<class T, class TYPE>
    class FileEncoder : public internal::CordaAvroSpec<T, TYPE> {
    private :
        std::string m_fileName;
    public :
        FileEncoder (std::string fileName_, const ::avro::ValidSchema &schema_)
                : internal::CordaAvroSpec<T, TYPE>(schema_), m_fileName { std::move(fileName_)}
        {
        }

        ~FileEncoder() override = default;

        ::avro::OutputStreamPtr stream() override {
            return ::avro::fileOutputStream (m_fileName.c_str());
        }
    };

    template<class T, class TYPE>
    class MemEncoder : public internal::CordaAvroSpec<T, TYPE> {
    public :
        explicit MemEncoder (const ::avro::ValidSchema &schema_)
                : internal::CordaAvroSpec<T, TYPE>(schema_) {
        }

        ~MemEncoder() override = default;

        ::avro::OutputStreamPtr stream() override {
            return ::avro::memoryOutputStream();
        }
    };
}

/**********************************************************************************************************************/

template<class T, class FORMAT>
::avro::OutputStreamPtr
corda::p2p::avro::internal::
CordaAvroSpec<T, FORMAT>::encode (const T & m_)  {
    std::unique_ptr<::avro::OutputStream> out = stream();
    auto e = internal::XCoder<FORMAT>::encoder (this->schema());
    e->init (*out);

    try {
        m_.encode(e);
    } catch (const std::exception & excp) {
        std::cerr << __FILE__ << "::" << __LINE__ << " - " << excp.what() << std::endl;
        throw excp;
    }

    out->flush();
    return out;
}

/**********************************************************************************************************************/

