#include "X500.h"

#include <sstream>
#include <exception>

corda::p2p::identity::
X500::X500()
    : m_commonName ("")
    , m_organizationUnit ("")
    , m_organizationName("")
    , m_localityName("")
    , m_stateName ("")
    , m_country ("")
{

}

const std::string &
corda::p2p::identity::
X500::commonName() const {
    return m_commonName;
}

const std::string &
corda::p2p::identity::
X500::organizationUnit() const {
    return m_organizationUnit;
}

const std::string &
corda::p2p::identity::
X500::organizationName() const {
    return m_organizationName;
}

const std::string &
corda::p2p::identity::
X500::localityName() const  {
    return m_localityName;
}

const std::string &
corda::p2p::identity::
X500::stateName() const {
    return m_stateName;
}

const std::string &
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
    if (m_commonName.size()) {
        ss << "CN=" << m_commonName;
        first = false;
    }
    if (m_organizationUnit.size()) {
        if (!first) {
            ss << ", ";
        }
        ss << "OU=" << m_organizationUnit;
        first = false;
    }
    if (m_organizationName.size()) {
        if (!first) {
            ss << ", ";
        }
        ss  << "O=" << m_organizationName;
        first = false;
    }
    if (m_localityName.size()) {
        if (!first) {
            ss << ", ";
        }
        ss << "L=" << m_localityName;
        first = false;
    }
    if (m_stateName.size()) {
        if (!first) {
            ss << ", ";
        }
        ss << "S=" << m_stateName;
        first = false;
    }
    if (m_country.size()) {
        if (!first) {
            ss << ", ";
        }
        ss << "C=" << m_country;
        first = false;
    }

    return ss.str();
}
