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


#ifndef ENVELOPE_H_3639524737__H_
#define ENVELOPE_H_3639524737__H_


#include <sstream>
#include "boost/any.hpp"
#include "avro/Specific.hh"
#include "avro/Encoder.hh"
#include "avro/Decoder.hh"

namespace net {
namespace corda {
namespace data {
struct AvroEnvelope {
    std::array<uint8_t, 8> magic;
    std::array<uint8_t, 32> fingerprint;
    int32_t flags;
    std::vector<uint8_t> payload;
    AvroEnvelope() :
        magic(std::array<uint8_t, 8>()),
        fingerprint(std::array<uint8_t, 32>()),
        flags(int32_t()),
        payload(std::vector<uint8_t>())
        { }
};

}
}
}
namespace avro {
template<> struct codec_traits<net::corda::data::AvroEnvelope> {
    static void encode(Encoder& e, const net::corda::data::AvroEnvelope& v) {
        avro::encode(e, v.magic);
        avro::encode(e, v.fingerprint);
        avro::encode(e, v.flags);
        avro::encode(e, v.payload);
    }
    static void decode(Decoder& d, net::corda::data::AvroEnvelope& v) {
        if (avro::ResolvingDecoder *rd =
            dynamic_cast<avro::ResolvingDecoder *>(&d)) {
            const std::vector<size_t> fo = rd->fieldOrder();
            for (std::vector<size_t>::const_iterator it = fo.begin();
                it != fo.end(); ++it) {
                switch (*it) {
                case 0:
                    avro::decode(d, v.magic);
                    break;
                case 1:
                    avro::decode(d, v.fingerprint);
                    break;
                case 2:
                    avro::decode(d, v.flags);
                    break;
                case 3:
                    avro::decode(d, v.payload);
                    break;
                default:
                    break;
                }
            }
        } else {
            avro::decode(d, v.magic);
            avro::decode(d, v.fingerprint);
            avro::decode(d, v.flags);
            avro::decode(d, v.payload);
        }
    }
};

}
#endif
