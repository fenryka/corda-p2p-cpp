#pragma once

#include <vector>
#include <string>
#include <avro/Stream.hh>
#include <avro/ValidSchema.hh>
#include <avro/Encoder.hh>
#include <array>

/**********************************************************************************************************************/

namespace corda::p2p::identity {

    class Identity;

}

/**********************************************************************************************************************/

namespace corda::p2p::avro {

    class CordaAvroFactory;

}

/**********************************************************************************************************************/

namespace corda::p2p::messaging {

    class Message {
    private :
        std::array<uint8_t, 8>  m_magic;
        std::vector<uint8_t>    m_payload;
    public :
        Message();

        virtual void encode (::avro::EncoderPtr) const = 0;
        virtual std::unique_ptr<avro::CordaAvroFactory> factory() = 0;
        virtual const std::array<uint8_t, 32> & fingerprint() = 0;

        [[nodiscard]] const decltype (m_magic) & magic() const { return m_magic; }
    };

    class Envelope : public Message {
    private :
        int                                  m_flags;
        std::shared_ptr<std::vector<uint8_t>> m_payload;
        /* The fingerprint of the type we're wrapping, not the fingperint of
           the Envelope */
        std::array<uint8_t, 32> m_fingerprint;

        static std::array<uint8_t, 32> s_fingerprint;

    public :
        Envelope (int, std::array<uint8_t, 32>, std::shared_ptr<std::vector<uint8_t>>);

        void encode (::avro::EncoderPtr) const override;
        std::unique_ptr<avro::CordaAvroFactory> factory() override;

        const std::array<uint8_t, 32> & fingerprint() override;
    };

    class AppMessage : public Message {
    private :
        std::vector<uint8_t>       m_payload;
        const identity::Identity & m_source;
        const identity::Identity & m_dest;

        static std::array<uint8_t, 32> s_fingerprint;
    public :
        AppMessage (
            std::vector<uint8_t>,
            const identity::Identity &,
            const identity::Identity &);

        void encode (::avro::EncoderPtr) const override;
        std::unique_ptr<avro::CordaAvroFactory> factory() override;
        const std::array<uint8_t, 32> & fingerprint() override;
    };

}

/**********************************************************************************************************************/
