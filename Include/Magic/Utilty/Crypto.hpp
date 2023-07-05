/*
 ******************************************************************************
 * @Author         : INotfound
 * @File           : Crypto.hpp
 * @Date           : 2023-07-03 18:29
 ******************************************************************************
 */
#pragma once

#include "Magic/Core/Core.hpp"

namespace Magic{
    /**
     * @brief MD5
     * @param str 需要加密的字符串
     * @return: 返回加密后的字符串
     */
    std::string MD5(const Magic::StringView& str);

    /**
     * @brief SHA1
     * @param str 需要加密的字符串
     * @return: 返回加密后的字符串
     */
    std::string SHA1(const Magic::StringView& str);

    /**
     * @brief MD5
     * @param str 需要加密的文件
     * @return: 返回文件加密后的十六进制字符串
     */
    std::string StringToHexMD5(const Magic::StringView& str);

    /**
     * @brief SHA1
     * @param str 需要加密的文件
     * @return: 返回文件加密后的十六进制字符串
     */
    std::string StringToHexSHA1(const Magic::StringView& str);

    /**
     * @brief Base64
     * @param str 需要解码的字符串
     * @return: 返回解码后的字符串
     */
    std::string Base64Decode(const Magic::StringView& src);

    /**
     * @brief Base64
     * @param src 需要编码的字符串
     * @return: 返回编码后的字符串
     */
    std::string Base64Encode(const Magic::StringView& src);

    /**
     * @brief MD5
     * @param filePath 需要加密的文件
     * @return: 返回文件加密后的十六进制字符串
     */
    std::string FileToHexMD5String(const Magic::StringView& filePath);
}