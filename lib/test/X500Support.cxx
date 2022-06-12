#include "X500Support.h"

corda::p2p::identity::test::
Alice::Alice()
    : X500()
{
    organizationName ("Alice");
    localityName ("London");
    country ("London");
}

corda::p2p::identity::test::
Bob::Bob()
    : X500()
{
    organizationName ("Bob");
    localityName ("London");
    country ("London");
}
