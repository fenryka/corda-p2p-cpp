#pragma once

#include <vector>
#include <string>
#include <avro/Stream.hh>
#include <avro/ValidSchema.hh>


namespace corda::p2p::identity {

    class Identity;

}



namespace corda::p2p::messaging {

    class Message {
    private :
        std::vector<uint8_t>                   m_payload;
        std::string                            m_key;
        const corda::p2p::identity::Identity & m_source;
        const avro::ValidSchema              & m_schema;

    public :
        Message() = delete;
        Message (const avro::ValidSchema &, std::string , const std::string &, const corda::p2p::identity::Identity &);

        std::unique_ptr <avro::OutputStream> encode (const corda::p2p::identity::Identity &);
    };

}