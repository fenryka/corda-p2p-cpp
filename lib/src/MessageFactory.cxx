#include "MessageFactory.h"

#include <fstream>
#include <Compiler.hh>

corda::p2p::messaging::
MessageFactory::MessageFactory(std::string schemaName_)
    : m_schemaName { std::move (schemaName_) }
{
    std::ifstream ifs { m_schemaName };
    avro::compileJsonSchema(ifs, m_schema);
}

std::unique_ptr<corda::p2p::messaging::Message>
corda::p2p::messaging::
MessageFactory::message(
        std::string  key_,
        const std::string & payload_,
        const corda::p2p::identity::Identity & source_
)  {
    return std::make_unique<corda::p2p::messaging::Message> (Message { m_schema, key_, payload_, source_ });
}
