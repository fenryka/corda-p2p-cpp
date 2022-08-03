#include <zlib.h>
#include <cassert>
#include <iostream>
#include <algorithm>

#include "Deflator.h"

#include <vector>

#define CHUNK 16384

class AutoZStream {
private :
    z_stream m_stream;

public :
    AutoZStream() = default;

    ~AutoZStream() {
        auto rtn = deflateEnd(&m_stream);
    }

    explicit operator z_stream() const { return m_stream; }

    z_stream & operator()() {
        return m_stream;
    }
};

/* Compress from file source to file dest until EOF on source.
   def() returns Z_OK on success, Z_MEM_ERROR if memory could not be
   allocated for processing, Z_STREAM_ERROR if an invalid compression
   level is supplied, Z_VERSION_ERROR if the version of zlib.h and the
   version of the library linked do not match, or Z_ERRNO if there is
   an error reading or writing the files. */
std::vector<uint8_t> deflate (std::vector<uint8_t> & source, int level)
{
    int flush;
    unsigned have;
    AutoZStream strm {};

    // lazy, but lets assume our compressed form can't be bigger than the uncompressed version
    uint8_t arr[CHUNK];

    /* allocate deflate state */
    strm().zalloc = Z_NULL;
    strm().zfree  = Z_NULL;
    strm().opaque = Z_NULL;

    int ret = deflateInit(&strm(), level);
    if (ret != Z_OK) throw std::runtime_error ("BAD Deflate INIT");


    strm().avail_in = source.size();
    strm().next_in = source.data();


    int offset { 0 };

    strm().avail_out = CHUNK;
    strm().next_out = arr;

    ret = deflate(&strm(), Z_FINISH);    /* no bad return value */
    std::cout << "err ?= " << ret << std::endl;

    assert(ret != Z_STREAM_ERROR);  /* state not clobbered */

    //assert (strm().avail_out == 0);
    assert(strm().avail_in == 0);     /* all input will be used */

    std::vector<uint8_t> rtn;
    rtn.insert (rtn.end(), arr, arr + strm().total_out);

    return std::move (rtn);
}


std::vector<uint8_t>
inflate (std::vector<uint8_t> & source_) {
    std::cout << __FUNCTION__ << "::" << source_.size() << std::endl;

    z_stream strm;

    /* allocate inflate state */
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.avail_in = 0;
    strm.next_in = Z_NULL;

    if (inflateInit(&strm) != Z_OK) {
        throw std::runtime_error("BAD INIT");
    }

    strm.avail_in = source_.size();
    strm.next_in = source_.data();

    uint8_t arr[CHUNK];

    strm.avail_out = CHUNK;
    strm.next_out = arr;


    std::vector<uint8_t> rtn;
    do {
        int ret = inflate(&strm, Z_FINISH);

        std::cout << strm.avail_in << " " << strm.avail_out << std::endl;
        std::cout << "err ?= " << ret << std::endl;


        std::cout << std::hex << (int)*arr << " " << (int)*(arr+1) << std::dec << std::endl;

        rtn.insert (rtn.end(), arr, arr+strm.total_out);
    } while (strm.avail_out == 0);

            /* clean up and return */
    //        (void)inflateEnd(&strm);

    return rtn;
}
