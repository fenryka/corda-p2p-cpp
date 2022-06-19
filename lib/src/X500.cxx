#include "X500.h"

#include <map>
#include <regex>

#include <sstream>
#include <exception>
#include <iostream>

corda::p2p::identity::
X500::X500() {

}

namespace {

    std::map<
            std::string,
            std::function<void(corda::p2p::identity::X500 &, const std::string &)>> setters = // NOLNT
    {
        {
                "CN",
                [](corda::p2p::identity::X500 & x500_, const std::string & value_)->void {
                    x500_.commonName (value_);
                }
        },
        {
                "OU",
                [](corda::p2p::identity::X500 & x500_, const std::string & value_)->void {
                    x500_.organizationUnit (value_);
                }
        },
        {
                "O",
                [](corda::p2p::identity::X500 & x500_, const std::string & value_)->void {
                    x500_.organizationName (value_);
                }
        },
        {
                "L",
                [](corda::p2p::identity::X500 & x500_, const std::string & value_)->void {
                    x500_.localityName (value_);
                }
        },
        {
                "S",
                [](corda::p2p::identity::X500 & x500_, const std::string & value_)->void {
                    x500_.stateName (value_);
                }
        },
        {
                "C",
                [](corda::p2p::identity::X500 & x500_, const std::string & value_)->void {
                    x500_.country (value_);
                }
        }
    };
}

corda::p2p::identity::
X500::X500 (const std::string & str_) {
    /*
     * Match the seperator between each element
     */
    const auto splitter = std::regex(R"(\s*,\s*)");
    const auto inner = std::regex ( R"((.*)\s*=\s*(.*))");

    std::sregex_token_iterator a ( str_.begin(), str_.end(), splitter, -1);
    while (a != std::sregex_token_iterator()) {
        std::match_results< std::string::const_iterator > mr;

        std::string s { *a++ };

        std::regex_search (s, mr, inner);
        setters[mr[1]] (*this, mr[2]);
    }
}

[[maybe_unused]] const std::string &
corda::p2p::identity::
X500::commonName() const {
    return m_commonName;
}

[[maybe_unused]] const std::string &
corda::p2p::identity::
X500::organizationUnit() const {
    return m_organizationUnit;
}

[[maybe_unused]] const std::string &
corda::p2p::identity::
X500::organizationName() const {
    return m_organizationName;
}

[[maybe_unused]] const std::string &
corda::p2p::identity::
X500::localityName() const  {
    return m_localityName;
}

[[maybe_unused]] const std::string &
corda::p2p::identity::
X500::stateName() const {
    return m_stateName;
}

[[maybe_unused]] const std::string &
corda::p2p::identity::
X500::country() const {
    return m_country;
}

void
corda::p2p::identity::
X500::commonName(const std::string & cn_) {
    m_commonName = cn_;
}

void
corda::p2p::identity::
X500::organizationUnit(const std::string & ou_) {
    m_organizationUnit = ou_;
}

void
corda::p2p::identity::
X500::organizationName(const std::string & on_) {
    m_organizationName = on_;
}

void
corda::p2p::identity::
X500::localityName(const std::string & l_) {
    m_localityName = l_;
}

void
corda::p2p::identity::
X500::stateName(const std::string & sn_) {
    m_stateName = sn_;
}

void
corda::p2p::identity::
X500::country(const std::string & c_) {
    m_country = c_;
}

std::string
corda::p2p::identity::
X500::name() const {
    bool first { true };

    std::stringstream ss;
    if (!m_commonName.empty()) {
        ss << "CN=" << m_commonName;
        first = false;
    }
    if (!m_organizationUnit.empty()) {
        if (!first) {
            ss << ", ";
        }
        ss << "OU=" << m_organizationUnit;
        first = false;
    }
    if (!m_organizationName.empty()) {
        if (!first) {
            ss << ", ";
        }
        ss  << "O=" << m_organizationName;
        first = false;
    }
    if (!m_localityName.empty()) {
        if (!first) {
            ss << ", ";
        }
        ss << "L=" << m_localityName;
        first = false;
    }
    if (!m_stateName.empty()) {
        if (!first) {
            ss << ", ";
        }
        ss << "S=" << m_stateName;
        first = false;
    }
    if (!m_country.empty()) {
        if (!first) {
            ss << ", ";
        }
        ss << "C=" << m_country;
        first = false;
    }

    return ss.str();
}
