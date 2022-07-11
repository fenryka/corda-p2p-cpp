#pragma once

#include <vector>
#include <string>
#include <avro/Stream.hh>
#include <avro/ValidSchema.hh>

/**********************************************************************************************************************/

namespace corda::p2p::messaging {

    class Message;

}

/**********************************************************************************************************************/

/**
 * Wrap the AVRO library to allow easy streaming
 */
namespace corda::p2p::avro {

    class CordaAvro {
    private :
        const ::avro::ValidSchema & m_schema;
    public :
        explicit CordaAvro (const ::avro::ValidSchema & schema_)
            : m_schema { schema_ }
        {

        }

        virtual ~CordaAvro() = default;

        virtual std::unique_ptr <::avro::OutputStream> encode (const messaging::Message &) = 0;

        [[nodiscard]] decltype(m_schema) & schema() const {
            return m_schema;
        }
    };

    class CordaAvroFactory {
    private :
        std::string         m_schemaName;
        ::avro::ValidSchema m_schema;
    public :
        explicit CordaAvroFactory (::avro::ValidSchema);

        std::unique_ptr<CordaAvro> encodeAsJSONToFile(const std::string &);
        std::unique_ptr<CordaAvro> encodeAsJSONToMem();
        std::unique_ptr<CordaAvro> encodeAsBinaryToFile(const std::string &);
        std::unique_ptr<CordaAvro> encodeAsBinaryToMem();
    };
}

/**********************************************************************************************************************/
