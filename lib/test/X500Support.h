#pragma once

#include "X500.h"

namespace corda::p2p::identity::test {

class Alice : public corda::p2p::identity::X500 {
public:
    Alice();
};

class Bob : public corda::p2p::identity::X500 {
public:
    Bob();
};

}