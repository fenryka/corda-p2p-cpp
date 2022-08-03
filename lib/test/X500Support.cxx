#include "X500Support.h"

corda::p2p::identity::test::
AliceX500::AliceX500()
    : X500()
{
    organizationName ("Alice");
    localityName ("London");
    country ("London");
}

corda::p2p::identity::test::
BobX500::BobX500()
    : X500()
{
    organizationName ("Bob");
    localityName ("London");
    country ("London");
}
