#pragma once

#include <vector>
#include <string>
#include <avro/Stream.hh>

namespace corda::p2p::messaging {

    class Message {
    private :
        std::vector<uint8_t> m_payload;
        std::string m_key;
    public :
        Message (std::string , const std::string &);

        std::unique_ptr <avro::OutputStream> encode();


    };

}