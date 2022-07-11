#pragma once

#include <utility>

#include "debug.h"
#include "CordaAvro.h"
#include "avro/Encoder.hh"
#include "Message.h"

/*
 * Our Auto Generated header based on the AVRO schema
 */
#include "avro.h"

namespace corda::p2p::avro {

    struct Binary{};
    struct Json {};

    namespace {
        template<typename T>
        struct Encoder { };

        template<>
        struct Encoder<Binary> {
            [[maybe_unused]] static ::avro::EncoderPtr encoder(const ::avro::ValidSchema & schema_) {
                return ::avro::validatingEncoder (schema_, ::avro::binaryEncoder());
            }
        };

        template<>
        struct Encoder<Json> {
            [[maybe_unused]] static ::avro::EncoderPtr encoder(const ::avro::ValidSchema &schema_) {
                return ::avro::jsonEncoder (schema_);
            }
        };
    }

    template<class Format>
    class CordaAvroImpl: public CordaAvro {
    public :
        explicit CordaAvroImpl (const ::avro::ValidSchema & schema_)
          : CordaAvro { schema_ }
        {

        }

        ~CordaAvroImpl() override = default;

        virtual std::unique_ptr<::avro::OutputStream> stream() = 0;

        std::unique_ptr<::avro::OutputStream> encode (const messaging::Message & m_) override {
            DBG ("Encode " << std::endl);
            std::unique_ptr<::avro::OutputStream> out = stream();
            auto e = Encoder<Format>::encoder (this->schema());
            e->init (*out);

            try {
                m_.encode(e);
            } catch (const std::exception & e) {
#ifdef P2P_DEBUG
                std::stringstream ss;
                ss << "ERROR: " << e.what();
                DBG (ss.str() << std::endl);
#endif
                throw e;
            }

            return out;
        }
    };

    template<class T>
    class FileMessage : public CordaAvroImpl<T>{
    private :
        std::string m_fileName;
    public :
        FileMessage(
                std::string  fileName_,
                const ::avro::ValidSchema & schema_)
            : CordaAvroImpl<T>(schema_)
            , m_fileName { std::move( fileName_) }
        {
        }

        ~FileMessage() override = default;

        std::unique_ptr<::avro::OutputStream> stream() override {
            return ::avro::fileOutputStream (m_fileName.c_str());
        }
    };

    template<class T>
    class MemMessage : public CordaAvroImpl<T>{
    public :
        explicit MemMessage(const ::avro::ValidSchema &schema_)
                : CordaAvroImpl<T>(schema_)
        {
        }

        ~MemMessage() override = default;

        std::unique_ptr<::avro::OutputStream> stream() override {
            return ::avro::memoryOutputStream();
        }
    };

}
