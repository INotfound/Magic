/*
 ******************************************************************************
 * @Author         : INotfound
 * @File           : Compress.hpp
 * @Date           : 2023-07-03 18:29
 ******************************************************************************
 */
#pragma once

#include "Magic/Core/Core.hpp"

namespace Magic{
#ifdef ZLIB
    /**
     * @brief GZip Decompression
     * @param compressedData - The Gzip Compressed Data
     * @param data - The Resulting Uncompressed Data (May Contain Binary Data)
     * @return - true On Success, false On Failure
     */
    bool GZipDecoder(const Magic::StringView& compressedData,std::string& data);

    /**
     * @brief GZip Compression
     * @param data - The Data To Compress
     * @param compressedData - The Resulting Gzip Compressed Data
     * @param level - The Gzip Compress Level -1 = Default, 0 = No Compression, 1= Worst/Fastest Compression, 9 = Best/Slowest Compression
     * @return - true On Success, false On Failure
     */
    bool GZipEncoder(const Magic::StringView& data,std::string& compressedData);
#endif
    /**
     * @brief Brotli Decompression
     * @param compressedData - The Brotli Compressed Data
     * @param data - The Resulting Uncompressed Data (May Contain Binary Data)
     * @return - true On Success, false On Failure
     */
    bool BrotliDecoder(const Magic::StringView& compressedData,std::string& data);
    /**
     * @brief Brotli Compression
     * @param data - The Data To Compress
     * @param compressedData - The Resulting Brotli Compressed Data
     * @return - true On Success, false On Failure
     */
    bool BrotliEncoder(const Magic::StringView& data,std::string& compressedData);
}
