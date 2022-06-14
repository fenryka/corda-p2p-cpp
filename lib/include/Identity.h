#pragma once

#include <memory>

namespace corda::p2p::identity {
    class X500;
}

namespace corda::p2p::identity {

    class Identity {
    private :
        std::unique_ptr<X500> m_name;

    public:
        explicit Identity (std::unique_ptr<X500> &);


    };

}
