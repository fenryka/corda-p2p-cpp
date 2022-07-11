#pragma once

#include <string>

/*
 * Because I *always* forget the abreviations
 *
 * CN=commonName
 * OU=organizationUnit
 * O=organizationName
 * L=localityName
 * S=stateName
 * C=country
 */

namespace corda::p2p::identity {

    class X500 {
    private :
        std::string m_commonName;
        std::string m_organizationUnit;
        std::string m_organizationName;
        std::string m_localityName;
        std::string m_stateName;
        std::string m_country;

    public :
        X500() = default;
        explicit X500 (const std::string &);

        [[maybe_unused]] [[nodiscard]] const std::string & commonName() const;

        [[maybe_unused]] [[maybe_unused]] [[nodiscard]] const std::string & organizationUnit() const;

        [[maybe_unused]] [[nodiscard]] const std::string & organizationName() const;

        [[maybe_unused]] [[nodiscard]] const std::string & localityName() const;

        [[maybe_unused]] [[nodiscard]] const std::string & stateName() const;

        [[maybe_unused]] [[nodiscard]] const std::string & country() const;

        [[maybe_unused]] [[nodiscard]] std::string name() const;

        [[maybe_unused]] void commonName(const std::string &);
        [[maybe_unused]] void organizationUnit(const std::string &);
        [[maybe_unused]] void organizationName(const std::string &);
        [[maybe_unused]] void localityName(const std::string &);
        [[maybe_unused]] void stateName(const std::string &);
        [[maybe_unused]] void country(const std::string &);
    };

}
