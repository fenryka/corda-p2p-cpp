/**
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements.  See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership.  The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License.  You may obtain a copy of the License at
 *
 *     https://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */


#ifndef AVRO_H_1935693503__H_
#define AVRO_H_1935693503__H_


#include <sstream>
#include "boost/any.hpp"
#include "avro/Specific.hh"
#include "avro/Encoder.hh"
#include "avro/Decoder.hh"

namespace net::corda::data::identity {
    struct HoldingIdentity {
        std::string x500Name;
        std::string groupId;

        HoldingIdentity() :
                x500Name(std::string()),
                groupId(std::string()) {}
    };
}

namespace net::corda::p2p::app {

struct UnauthenticatedMessageHeader {
    data::identity::HoldingIdentity destination;
    data::identity::HoldingIdentity /**/source;
    std::string subsystem;
    UnauthenticatedMessageHeader() :
        destination(data::identity::HoldingIdentity()),
        source(data::identity::HoldingIdentity()),
        subsystem(std::string())
        { }
};

struct _avro_asvc_Union__0__ {
private:
    size_t idx_;
    boost::any value_;
public:
    size_t idx() const { return idx_; }
    bool is_null() const {
        return (idx_ == 0);
    }
    void set_null() {
        idx_ = 0;
        value_ = boost::any();
    }
    int64_t get_long() const;
    void set_long(const int64_t& v);
    _avro_asvc_Union__0__();
};

struct AuthenticatedMessageHeader {
    typedef _avro_asvc_Union__0__ ttl_t;
    data::identity::HoldingIdentity destination;
    data::identity::HoldingIdentity source;
    ttl_t ttl;
    std::string messageId;
    std::string traceId;
    std::string subsystem;
    AuthenticatedMessageHeader() :
        destination(data::identity::HoldingIdentity()),
        source(data::identity::HoldingIdentity()),
        ttl(ttl_t()),
        messageId(std::string()),
        traceId(std::string()),
        subsystem(std::string())
        { }
};

struct UnauthenticatedMessage {
    UnauthenticatedMessageHeader header;
    std::vector<uint8_t> payload;
    UnauthenticatedMessage() :
        header(UnauthenticatedMessageHeader()),
        payload(std::vector<uint8_t>())
        { }
};

struct AuthenticatedMessage {
    AuthenticatedMessageHeader header;
    std::vector<uint8_t> payload;
    AuthenticatedMessage() :
        header(AuthenticatedMessageHeader()),
        payload(std::vector<uint8_t>())
        { }
};

struct _avro_asvc_Union__1__ {
private:
    size_t idx_;
    boost::any value_;
public:
    size_t idx() const { return idx_; }
    AuthenticatedMessage get_AuthenticatedMessage() const;
    void set_AuthenticatedMessage(const AuthenticatedMessage& v);
    UnauthenticatedMessage get_UnauthenticatedMessage() const;
    void set_UnauthenticatedMessage(const UnauthenticatedMessage& v);
    _avro_asvc_Union__1__();
};

struct AppMessage {
    typedef _avro_asvc_Union__1__ message_t;
    message_t message;
    AppMessage() :
        message(message_t())
        { }
};

struct _avro_asvc_Union__2__ {
private:
    size_t idx_;
    boost::any value_;
public:
    size_t idx() const { return idx_; }
    data::identity::HoldingIdentity get_HoldingIdentity() const;
    void set_HoldingIdentity(const data::identity::HoldingIdentity& v);
    UnauthenticatedMessageHeader get_UnauthenticatedMessageHeader() const;
    void set_UnauthenticatedMessageHeader(const UnauthenticatedMessageHeader& v);
    AuthenticatedMessageHeader get_AuthenticatedMessageHeader() const;
    void set_AuthenticatedMessageHeader(const AuthenticatedMessageHeader& v);
    UnauthenticatedMessage get_UnauthenticatedMessage() const;
    void set_UnauthenticatedMessage(const UnauthenticatedMessage& v);
    AuthenticatedMessage get_AuthenticatedMessage() const;
    void set_AuthenticatedMessage(const AuthenticatedMessage& v);
    AppMessage get_AppMessage() const;
    void set_AppMessage(const AppMessage& v);
    _avro_asvc_Union__2__();
};

inline
int64_t _avro_asvc_Union__0__::get_long() const {
    if (idx_ != 1) {
        throw avro::Exception("Invalid type for union");
    }
    return boost::any_cast<int64_t >(value_);
}

inline
void _avro_asvc_Union__0__::set_long(const int64_t& v) {
    idx_ = 1;
    value_ = v;
}

inline
AuthenticatedMessage _avro_asvc_Union__1__::get_AuthenticatedMessage() const {
    if (idx_ != 0) {
        throw avro::Exception("Invalid type for union");
    }
    return boost::any_cast<AuthenticatedMessage >(value_);
}

inline
void _avro_asvc_Union__1__::set_AuthenticatedMessage(const AuthenticatedMessage& v) {
    idx_ = 0;
    value_ = v;
}

inline
UnauthenticatedMessage _avro_asvc_Union__1__::get_UnauthenticatedMessage() const {
    if (idx_ != 1) {
        throw avro::Exception("Invalid type for union");
    }
    return boost::any_cast<UnauthenticatedMessage >(value_);
}

inline
void _avro_asvc_Union__1__::set_UnauthenticatedMessage(const UnauthenticatedMessage& v) {
    idx_ = 1;
    value_ = v;
}

inline
data::identity::HoldingIdentity _avro_asvc_Union__2__::get_HoldingIdentity() const {
    if (idx_ != 0) {
        throw avro::Exception("Invalid type for union");
    }
    return boost::any_cast<data::identity::HoldingIdentity >(value_);
}

inline
void _avro_asvc_Union__2__::set_HoldingIdentity(const data::identity::HoldingIdentity& v) {
    idx_ = 0;
    value_ = v;
}

inline
UnauthenticatedMessageHeader _avro_asvc_Union__2__::get_UnauthenticatedMessageHeader() const {
    if (idx_ != 1) {
        throw avro::Exception("Invalid type for union");
    }
    return boost::any_cast<UnauthenticatedMessageHeader >(value_);
}

inline
void _avro_asvc_Union__2__::set_UnauthenticatedMessageHeader(const UnauthenticatedMessageHeader& v) {
    idx_ = 1;
    value_ = v;
}

inline
AuthenticatedMessageHeader _avro_asvc_Union__2__::get_AuthenticatedMessageHeader() const {
    if (idx_ != 2) {
        throw avro::Exception("Invalid type for union");
    }
    return boost::any_cast<AuthenticatedMessageHeader >(value_);
}

inline
void _avro_asvc_Union__2__::set_AuthenticatedMessageHeader(const AuthenticatedMessageHeader& v) {
    idx_ = 2;
    value_ = v;
}

inline
UnauthenticatedMessage _avro_asvc_Union__2__::get_UnauthenticatedMessage() const {
    if (idx_ != 3) {
        throw avro::Exception("Invalid type for union");
    }
    return boost::any_cast<UnauthenticatedMessage >(value_);
}

inline
void _avro_asvc_Union__2__::set_UnauthenticatedMessage(const UnauthenticatedMessage& v) {
    idx_ = 3;
    value_ = v;
}

inline
AuthenticatedMessage _avro_asvc_Union__2__::get_AuthenticatedMessage() const {
    if (idx_ != 4) {
        throw avro::Exception("Invalid type for union");
    }
    return boost::any_cast<AuthenticatedMessage >(value_);
}

inline
void _avro_asvc_Union__2__::set_AuthenticatedMessage(const AuthenticatedMessage& v) {
    idx_ = 4;
    value_ = v;
}

inline
AppMessage _avro_asvc_Union__2__::get_AppMessage() const {
    if (idx_ != 5) {
        throw avro::Exception("Invalid type for union");
    }
    return boost::any_cast<AppMessage >(value_);
}

inline
void _avro_asvc_Union__2__::set_AppMessage(const AppMessage& v) {
    idx_ = 5;
    value_ = v;
}

inline _avro_asvc_Union__0__::_avro_asvc_Union__0__() : idx_(0) { }
inline _avro_asvc_Union__1__::_avro_asvc_Union__1__() : idx_(0), value_(AuthenticatedMessage()) { }
inline _avro_asvc_Union__2__::_avro_asvc_Union__2__() : idx_(0), value_(data::identity::HoldingIdentity()) { }
}

namespace avro {
template<> struct codec_traits<net::corda::data::identity::HoldingIdentity> {
    static void encode(Encoder& e, const net::corda::data::identity::HoldingIdentity& v) {
        avro::encode(e, v.x500Name);
        avro::encode(e, v.groupId);
    }
    static void decode(Decoder& d, net::corda::data::identity::HoldingIdentity& v) {
        if (avro::ResolvingDecoder *rd =
            dynamic_cast<avro::ResolvingDecoder *>(&d)) {
            const std::vector<size_t> fo = rd->fieldOrder();
            for (std::vector<size_t>::const_iterator it = fo.begin();
                it != fo.end(); ++it) {
                switch (*it) {
                case 0:
                    avro::decode(d, v.x500Name);
                    break;
                case 1:
                    avro::decode(d, v.groupId);
                    break;
                default:
                    break;
                }
            }
        } else {
            avro::decode(d, v.x500Name);
            avro::decode(d, v.groupId);
        }
    }
};

template<> struct codec_traits<net::corda::p2p::app::UnauthenticatedMessageHeader> {
    static void encode(Encoder& e, const net::corda::p2p::app::UnauthenticatedMessageHeader& v) {
        avro::encode(e, v.destination);
        avro::encode(e, v.source);
        avro::encode(e, v.subsystem);
    }
    static void decode(Decoder& d, net::corda::p2p::app::UnauthenticatedMessageHeader& v) {
        if (avro::ResolvingDecoder *rd =
            dynamic_cast<avro::ResolvingDecoder *>(&d)) {
            const std::vector<size_t> fo = rd->fieldOrder();
            for (std::vector<size_t>::const_iterator it = fo.begin();
                it != fo.end(); ++it) {
                switch (*it) {
                case 0:
                    avro::decode(d, v.destination);
                    break;
                case 1:
                    avro::decode(d, v.source);
                    break;
                case 2:
                    avro::decode(d, v.subsystem);
                    break;
                default:
                    break;
                }
            }
        } else {
            avro::decode(d, v.destination);
            avro::decode(d, v.source);
            avro::decode(d, v.subsystem);
        }
    }
};

template<> struct codec_traits<net::corda::p2p::app::_avro_asvc_Union__0__> {
    static void encode(Encoder& e, net::corda::p2p::app::_avro_asvc_Union__0__ v) {
        e.encodeUnionIndex(v.idx());
        switch (v.idx()) {
        case 0:
            e.encodeNull();
            break;
        case 1:
            avro::encode(e, v.get_long());
            break;
        }
    }
    static void decode(Decoder& d, net::corda::p2p::app::_avro_asvc_Union__0__& v) {
        size_t n = d.decodeUnionIndex();
        if (n >= 2) { throw avro::Exception("Union index too big"); }
        switch (n) {
        case 0:
            d.decodeNull();
            v.set_null();
            break;
        case 1:
            {
                int64_t vv;
                avro::decode(d, vv);
                v.set_long(vv);
            }
            break;
        }
    }
};

template<> struct codec_traits<net::corda::p2p::app::AuthenticatedMessageHeader> {
    static void encode(Encoder& e, const net::corda::p2p::app::AuthenticatedMessageHeader& v) {
        avro::encode(e, v.destination);
        avro::encode(e, v.source);
        avro::encode(e, v.ttl);
        avro::encode(e, v.messageId);
        avro::encode(e, v.traceId);
        avro::encode(e, v.subsystem);
    }
    static void decode(Decoder& d, net::corda::p2p::app::AuthenticatedMessageHeader& v) {
        if (avro::ResolvingDecoder *rd =
            dynamic_cast<avro::ResolvingDecoder *>(&d)) {
            const std::vector<size_t> fo = rd->fieldOrder();
            for (std::vector<size_t>::const_iterator it = fo.begin();
                it != fo.end(); ++it) {
                switch (*it) {
                case 0:
                    avro::decode(d, v.destination);
                    break;
                case 1:
                    avro::decode(d, v.source);
                    break;
                case 2:
                    avro::decode(d, v.ttl);
                    break;
                case 3:
                    avro::decode(d, v.messageId);
                    break;
                case 4:
                    avro::decode(d, v.traceId);
                    break;
                case 5:
                    avro::decode(d, v.subsystem);
                    break;
                default:
                    break;
                }
            }
        } else {
            avro::decode(d, v.destination);
            avro::decode(d, v.source);
            avro::decode(d, v.ttl);
            avro::decode(d, v.messageId);
            avro::decode(d, v.traceId);
            avro::decode(d, v.subsystem);
        }
    }
};

template<> struct codec_traits<net::corda::p2p::app::UnauthenticatedMessage> {
    static void encode(Encoder& e, const net::corda::p2p::app::UnauthenticatedMessage& v) {
        avro::encode(e, v.header);
        avro::encode(e, v.payload);
    }
    static void decode(Decoder& d, net::corda::p2p::app::UnauthenticatedMessage& v) {
        if (avro::ResolvingDecoder *rd =
            dynamic_cast<avro::ResolvingDecoder *>(&d)) {
            const std::vector<size_t> fo = rd->fieldOrder();
            for (std::vector<size_t>::const_iterator it = fo.begin();
                it != fo.end(); ++it) {
                switch (*it) {
                case 0:
                    avro::decode(d, v.header);
                    break;
                case 1:
                    avro::decode(d, v.payload);
                    break;
                default:
                    break;
                }
            }
        } else {
            avro::decode(d, v.header);
            avro::decode(d, v.payload);
        }
    }
};

template<> struct codec_traits<net::corda::p2p::app::AuthenticatedMessage> {
    static void encode(Encoder& e, const net::corda::p2p::app::AuthenticatedMessage& v) {
        avro::encode(e, v.header);
        avro::encode(e, v.payload);
    }
    static void decode(Decoder& d, net::corda::p2p::app::AuthenticatedMessage& v) {
        if (avro::ResolvingDecoder *rd =
            dynamic_cast<avro::ResolvingDecoder *>(&d)) {
            const std::vector<size_t> fo = rd->fieldOrder();
            for (std::vector<size_t>::const_iterator it = fo.begin();
                it != fo.end(); ++it) {
                switch (*it) {
                case 0:
                    avro::decode(d, v.header);
                    break;
                case 1:
                    avro::decode(d, v.payload);
                    break;
                default:
                    break;
                }
            }
        } else {
            avro::decode(d, v.header);
            avro::decode(d, v.payload);
        }
    }
};

template<> struct codec_traits<net::corda::p2p::app::_avro_asvc_Union__1__> {
    static void encode(Encoder& e, net::corda::p2p::app::_avro_asvc_Union__1__ v) {
        e.encodeUnionIndex(v.idx());
        switch (v.idx()) {
        case 0:
            avro::encode(e, v.get_AuthenticatedMessage());
            break;
        case 1:
            avro::encode(e, v.get_UnauthenticatedMessage());
            break;
        }
    }
    static void decode(Decoder& d, net::corda::p2p::app::_avro_asvc_Union__1__& v) {
        size_t n = d.decodeUnionIndex();
        if (n >= 2) { throw avro::Exception("Union index too big"); }
        switch (n) {
        case 0:
            {
                net::corda::p2p::app::AuthenticatedMessage vv;
                avro::decode(d, vv);
                v.set_AuthenticatedMessage(vv);
            }
            break;
        case 1:
            {
                net::corda::p2p::app::UnauthenticatedMessage vv;
                avro::decode(d, vv);
                v.set_UnauthenticatedMessage(vv);
            }
            break;
        }
    }
};

template<> struct codec_traits<net::corda::p2p::app::AppMessage> {
    static void encode(Encoder& e, const net::corda::p2p::app::AppMessage& v) {
        avro::encode(e, v.message);
    }
    static void decode(Decoder& d, net::corda::p2p::app::AppMessage& v) {
        if (avro::ResolvingDecoder *rd =
            dynamic_cast<avro::ResolvingDecoder *>(&d)) {
            const std::vector<size_t> fo = rd->fieldOrder();
            for (std::vector<size_t>::const_iterator it = fo.begin();
                it != fo.end(); ++it) {
                switch (*it) {
                case 0:
                    avro::decode(d, v.message);
                    break;
                default:
                    break;
                }
            }
        } else {
            avro::decode(d, v.message);
        }
    }
};

template<> struct codec_traits<net::corda::p2p::app::_avro_asvc_Union__2__> {
    static void encode(Encoder& e, net::corda::p2p::app::_avro_asvc_Union__2__ v) {
        e.encodeUnionIndex(v.idx());
        switch (v.idx()) {
        case 0:
            avro::encode(e, v.get_HoldingIdentity());
            break;
        case 1:
            avro::encode(e, v.get_UnauthenticatedMessageHeader());
            break;
        case 2:
            avro::encode(e, v.get_AuthenticatedMessageHeader());
            break;
        case 3:
            avro::encode(e, v.get_UnauthenticatedMessage());
            break;
        case 4:
            avro::encode(e, v.get_AuthenticatedMessage());
            break;
        case 5:
            avro::encode(e, v.get_AppMessage());
            break;
        }
    }
    static void decode(Decoder& d, net::corda::p2p::app::_avro_asvc_Union__2__& v) {
        size_t n = d.decodeUnionIndex();
        if (n >= 6) { throw avro::Exception("Union index too big"); }
        switch (n) {
        case 0:
            {
                net::corda::data::identity::HoldingIdentity vv;
                avro::decode(d, vv);
                v.set_HoldingIdentity(vv);
            }
            break;
        case 1:
            {
                net::corda::p2p::app::UnauthenticatedMessageHeader vv;
                avro::decode(d, vv);
                v.set_UnauthenticatedMessageHeader(vv);
            }
            break;
        case 2:
            {
                net::corda::p2p::app::AuthenticatedMessageHeader vv;
                avro::decode(d, vv);
                v.set_AuthenticatedMessageHeader(vv);
            }
            break;
        case 3:
            {
                net::corda::p2p::app::UnauthenticatedMessage vv;
                avro::decode(d, vv);
                v.set_UnauthenticatedMessage(vv);
            }
            break;
        case 4:
            {
                net::corda::p2p::app::AuthenticatedMessage vv;
                avro::decode(d, vv);
                v.set_AuthenticatedMessage(vv);
            }
            break;
        case 5:
            {
                net::corda::p2p::app::AppMessage vv;
                avro::decode(d, vv);
                v.set_AppMessage(vv);
            }
            break;
        }
    }
};

}
#endif
