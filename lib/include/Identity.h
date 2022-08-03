#pragma once

#include <memory>

#include <X500.h>

namespace corda::p2p::identity {

    class Identity {
    private :
        std::unique_ptr<X500> m_name;

    public:
        explicit Identity (std::unique_ptr<X500>);
        explicit Identity (const std::string &_);

        [[nodiscard]] std::string name() const;
    };

}
