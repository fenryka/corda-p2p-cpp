#pragma once

#include <string>
#include <ValidSchema.hh>

#include "Message.h"

namespace corda::p2p::messaging {

    class MessageFactory {
    private :
        std::string       m_schemaName;
        avro::ValidSchema m_schema;

    public :
        explicit MessageFactory (std::string);

        std::unique_ptr<Message> message(
            std::string  key_,
            const std::string & payload_,
            const corda::p2p::identity::Identity & source_);
    };

}