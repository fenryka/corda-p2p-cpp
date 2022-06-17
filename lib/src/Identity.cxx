#include "Identity.h"

#include "X500.h"

[[maybe_unused]] corda::p2p::identity::
Identity::Identity (std::unique_ptr<corda::p2p::identity::X500> & name_)
    : m_name(std::move (name_))
{

}

std::string
corda::p2p::identity::
Identity::name() const {
    return m_name->name();
}
