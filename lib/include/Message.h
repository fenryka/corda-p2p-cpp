#pragma once

#include <vector>
#include <string>
#include <avro/Stream.hh>
#include <avro/ValidSchema.hh>

namespace corda::p2p::identity {

    class Identity;

}

namespace corda::p2p::messaging {

    class BaseMessage {
    private :
        std::vector<uint8_t>                   m_payload;
        std::string                            m_key;
        const corda::p2p::identity::Identity & m_source;
        const avro::ValidSchema              & m_schema;
    public :
        BaseMessage() = delete;
        BaseMessage (const avro::ValidSchema &, std::string , const std::string &, const corda::p2p::identity::Identity &);
        virtual ~BaseMessage() = default;

        virtual std::unique_ptr <avro::OutputStream> encode (const corda::p2p::identity::Identity &) = 0;

        [[nodiscard]] decltype(m_source) & source() const {
            return m_source;
        }

        [[nodiscard]] decltype(m_schema) & schema() const {
            return m_schema;
        }

        [[nodiscard]] const decltype(m_payload) & payload() const {
            return m_payload;
        }

        [[nodiscard]] const decltype(m_key) & key() const {
            return m_key;
        }
    };

    template<class Encoder>
    class Message : public BaseMessage, public Encoder {
    public :
        Message (
                const avro::ValidSchema & schema_,
                std::string  key_,
                const std::string & payload_,
                const corda::p2p::identity::Identity & source_)
            : BaseMessage (schema_, key_, payload_, source_)
            , Encoder { }
        { }

        std::unique_ptr <avro::OutputStream> encode (const corda::p2p::identity::Identity & to_) override {
            return Encoder::encode_ (*this, to_);
        }
    };

    class MemEncoder {
    public :
        MemEncoder() = default;

        std::unique_ptr<avro::OutputStream> encode_ (const BaseMessage &, const identity::Identity & to_);
    };

    class FileEncoder {
    private :
        std::string m_fileName;
    public :
        FileEncoder() = default;

        std::unique_ptr<avro::OutputStream> encode_ (const BaseMessage &, const identity::Identity & to_);

        void fileName (const std::string & fileName_) {
            m_fileName = fileName_;
        }
    };

}