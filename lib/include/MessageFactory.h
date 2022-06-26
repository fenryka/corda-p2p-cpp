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

        std::unique_ptr<BaseMessage> messageMemEncode(
            std::string,
            const std::string &,
            const identity::Identity &);

        std::unique_ptr<BaseMessage> messageFileEncode(
                const std::string &,
                std::string,
                const std::string &,
                const corda::p2p::identity::Identity &);
    };

}