#pragma once
#ifdef ZLIB
#include <string>
namespace Magic{
    /**
     * @brief GZip Decompression
     * @param compressedData - the gzip compressed data
     * @param data - the resulting uncompressed data (may contain binary data)
     * @return - true on success, false on failure
     */
    bool GzipUnCompress(const std::string& compressedData, std::string& data);
    /**
     * GZip Compression
     * @param data - the data to compress (does not have to be string, can be binary data)
     * @param compressedData - the resulting gzip compressed data
     * @param level - the gzip compress level -1 = default, 0 = no compression, 1= worst/fastest compression, 9 = best/slowest compression
     * @return - true on success, false on failure
     */
    bool GzipCompress(const std::string& data, std::string& compressedData, int level = -1);
}
#endif