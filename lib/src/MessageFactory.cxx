#include "MessageFactory.h"

#include <fstream>
#include <avro/Schema.hh>

#include "SchemaBuilder.h"

/**
 *
 * @param schemaName_ This is supposed to contain the file name of our JSON schema to allow
 * us dynamic parsing. However, that isn't possible given the current state of the C++ AVRO
 * spec so we resort to our in-code schema builder and leave this here when that library
 * catches up with our needs.
 */
corda::p2p::messaging::
MessageFactory::MessageFactory(std::string schemaName_)
    : m_schemaName { std::move (schemaName_) }
    , m_schema (avro::ValidSchema (corda::p2p::messaging::buildAppMessageSchema()))
{
}

std::unique_ptr<corda::p2p::messaging::Message>
corda::p2p::messaging::
MessageFactory::message(
        std::string  key_,
        const std::string & payload_,
        const corda::p2p::identity::Identity & source_
)  {
    return std::make_unique<corda::p2p::messaging::Message> (Message { m_schema, std::move (key_), payload_, source_ });
}
