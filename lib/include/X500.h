#pragma once

#include <string>

/*
 * CN=commonName
 * OU=organizationUnit
 * O=organizationName
 * L=localityName
 * S=stateName
 * C=country
 */

namespace corda { namespace p2p { namespace identity {

    class X500 {
    private :
        std::string m_commonName;
        std::string m_organizationUnit;
        std::string m_organizationName;
        std::string m_localityName;
        std::string m_stateName;
        std::string m_country;

    public :
        X500();

        const std::string & commonName() const;
        const std::string & organizationUnit() const;
        const std::string & organizationName() const;
        const std::string & localityName() const;
        const std::string & stateName() const;
        const std::string & country() const;

        std::string name() const;

        void commonName(const std::string &);
        void organizationUnit(const std::string &);
        void organizationName(const std::string &);
        void localityName(const std::string &);
        void stateName(const std::string &);
        void country(const std::string &);
    };

} } }
