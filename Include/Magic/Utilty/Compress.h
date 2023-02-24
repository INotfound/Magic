#pragma once

#include <string>

namespace Magic{
#ifdef ZLIB
    /**
     * @brief GZip Decompression
     * @param compressedData - the gzip compressed data
     * @param data - the resulting uncompressed data (may contain binary data)
     * @return - true on success, false on failure
     */
    bool GZipDecoder(const std::string& compressedData,std::string& data);

    /**
     * GZip Compression
     * @param data - the data to compress (does not have to be string, can be binary data)
     * @param compressedData - the resulting gzip compressed data
     * @param level - the gzip compress level -1 = default, 0 = no compression, 1= worst/fastest compression, 9 = best/slowest compression
     * @return - true on success, false on failure
     */
    bool GZipEncoder(const std::string& data,std::string& compressedData);
#endif

    bool BrotliDecoder(const std::string& compressedData,std::string& data);

    bool BrotliEncoder(const std::string& data,std::string& compressedData);
}
