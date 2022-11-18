#pragma once

#include "X500.h"
#include "Identity.h"

namespace corda::p2p::identity::test {

    class AliceX500 : public corda::p2p::identity::X500 {
    public:
        AliceX500();

        static std::unique_ptr<X500> ptr() {
            return std::make_unique<AliceX500>();
        };
    };

    class BobX500 : public corda::p2p::identity::X500 {
    public:
        BobX500();

        static std::unique_ptr<X500> ptr() {
            return std::make_unique<BobX500>();
        };
    };

}


namespace corda::p2p::identity::test {

    class Alice : public corda::p2p::identity::Identity {
    public :
        Alice()
                : corda::p2p::identity::Identity (AliceX500::ptr())
        {

        }

        static std::shared_ptr<Alice> ptr() {
            return std::make_shared<Alice> (Alice());
        }
    };


    class Bob : public corda::p2p::identity::Identity {
    public :
        Bob()
            : corda::p2p::identity::Identity (BobX500::ptr())
        {

        }

        static std::shared_ptr<Bob> ptr() {
            return std::make_shared<Bob> (Bob());
        }
    };

}