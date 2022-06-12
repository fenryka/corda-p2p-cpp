#include "identity.h"

#include "X500.h"

corda::p2p::identity::
Identity::Identity (std::unique_ptr<corda::p2p::identity::X500> & name_)
    : m_name(std::move (name_))
{

}

