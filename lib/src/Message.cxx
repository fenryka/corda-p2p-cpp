#include <utility>

#include "Message.h"

corda::p2p::messaging::
Message::Message (std::string  key_, const std::string & payload_)
    : m_key(std::move(key_))
    , m_payload { payload_.begin(), payload_.end() }
{

}

std::unique_ptr <avro::OutputStream>
corda::p2p::messaging::
Message::encode() {

}
