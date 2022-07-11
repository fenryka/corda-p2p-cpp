#include "CordaAvro.h"

#include "CordaAvroSpecialisations.h"

corda::p2p::avro::
CordaAvroFactory::CordaAvroFactory(::avro::ValidSchema schema_)
        : m_schema (std::move (schema_))
{
}

std::unique_ptr<corda::p2p::avro::CordaAvro>
corda::p2p::avro::
CordaAvroFactory::encodeAsJSONToFile(const std::string & fileName_) {
    return std::make_unique<corda::p2p::avro::FileMessage<Json>> (
            FileMessage<Json> { fileName_, m_schema } );
}

std::unique_ptr<corda::p2p::avro::CordaAvro>
corda::p2p::avro::
CordaAvroFactory::encodeAsJSONToMem() {
    return std::make_unique<corda::p2p::avro::MemMessage<Json>> (
            MemMessage<Json> { m_schema } );
}

std::unique_ptr<corda::p2p::avro::CordaAvro>
corda::p2p::avro::
CordaAvroFactory::encodeAsBinaryToFile (const std::string & fileName_) {
    return std::make_unique<corda::p2p::avro::FileMessage<Binary>> (
            FileMessage<Binary> { fileName_, m_schema } );
}

std::unique_ptr<corda::p2p::avro::CordaAvro>
corda::p2p::avro::
CordaAvroFactory::encodeAsBinaryToMem() {
    return std::make_unique<corda::p2p::avro::MemMessage<Binary>> (
            MemMessage<Binary> { m_schema } );
}
