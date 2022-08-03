#pragma once

#include <vector>
#include <string>
#include <avro/Stream.hh>
#include <avro/Schema.hh>
#include <avro/Encoder.hh>
#include <avro/Decoder.hh>
#include <array>
#include <map>

/**********************************************************************************************************************/

namespace corda::p2p::identity {

        class Identity;

}

/**********************************************************************************************************************/

namespace corda::p2p::messaging {

    class CordaAvroSerializable {
    public :
        CordaAvroSerializable() = default;

        virtual void encode(::avro::EncoderPtr) const = 0;

        virtual const std::array<uint8_t, 32> & fingerprint() = 0;
    };
}

/**********************************************************************************************************************/

namespace corda::p2p::messaging {

    class Envelope : public CordaAvroSerializable {
    private :
        std::array<uint8_t, 8>                m_magic;
        int                                   m_flags{};
        std::shared_ptr<std::vector<uint8_t>> m_payload;

        /* The fingerprint of the type we're wrapping, not the fingerprint of
           the Envelope */
        std::array<uint8_t, 32> m_fingerprint{};

        static std::array<uint8_t, 32> s_fingerprint;

        /**
         * Purposefully left private to allow only the
         */
        Envelope();
    public :
        Envelope (int, std::array<uint8_t, 32>, std::shared_ptr<std::vector<uint8_t>>);

        void encode(::avro::EncoderPtr) const override;
        static std::unique_ptr<Envelope> decode (::avro::DecoderPtr);

        const std::array<uint8_t, 32> &fingerprint() override;
        [[ nodiscard ]] static ::avro::Schema schema();

        decltype(m_payload) payload() { return m_payload; }
        [[nodiscard]] decltype(m_flags) flags() const { return m_flags; }
        [[nodiscard]] const decltype(m_magic) & magic() const { return m_magic; }
        [[nodiscard]] const decltype(m_fingerprint) & payloadFingerprint() const { return m_fingerprint; }


        std::vector<uint8_t> decompress();
    };
}

/**********************************************************************************************************************/

namespace corda::p2p::messaging {

    class AppMessage : public CordaAvroSerializable {
    private :
        std::vector<uint8_t>                m_payload;
        std::shared_ptr<identity::Identity> m_source;
        std::shared_ptr<identity::Identity> m_destination;

        static std::map<std::string, std::array<uint8_t, 32>> s_fingerprints;

        AppMessage();
    public :
        AppMessage (
            std::vector<uint8_t>,
            std::shared_ptr<identity::Identity>,
            std::shared_ptr<identity::Identity>);

        [[ nodiscard ]] const std::array<uint8_t, 32> & fingerprint() override;
        [[ nodiscard ]] static ::avro::Schema schema();

        void encode (::avro::EncoderPtr) const override;
        static std::unique_ptr<AppMessage> decode (const ::avro::DecoderPtr&);
        const decltype(m_payload) & payload() { return m_payload; }

        std::vector<uint8_t> decompressedPayload();
    };

}

/**********************************************************************************************************************/
