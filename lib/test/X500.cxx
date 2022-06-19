#include <gtest/gtest.h>

#include "X500.h"

using namespace corda::p2p::identity;

TEST (x500, basic) { // NOLINT
    auto x1 = X500("CN=Dave");
    ASSERT_EQ("Dave", x1.commonName());
    auto x2 = X500("CN=Dave, O=Thing");
    ASSERT_EQ("Dave", x2.commonName());
    ASSERT_EQ("Thing", x2.organizationName());
}
