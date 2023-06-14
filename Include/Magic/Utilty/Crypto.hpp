/*
 * @File: Crypto.hpp
 * @Author: INotFound
 * @Date: 2020-03-13 23:00:00
 * @LastEditTime: 2020-03-14 00:09:01
 */
#pragma once

#include "Magic/Core/Core.hpp"

namespace Magic{
    /**
     * @brief MD5
     * @param str 需要加密的字符串
     * @return: 返回加密后的字符串
     */
    std::string MD5(const std::string_view& str);

    /**
     * @brief SHA1
     * @param str 需要加密的字符串
     * @return: 返回加密后的字符串
     */
    std::string SHA1(const std::string_view& str);

    /**
     * @brief MD5
     * @param str 需要加密的文件
     * @return: 返回文件加密后的十六进制字符串
     */
    std::string StringToHexMD5(const std::string_view& str);

    /**
     * @brief SHA1
     * @param str 需要加密的文件
     * @return: 返回文件加密后的十六进制字符串
     */
    std::string StringToHexSHA1(const std::string_view& str);

    /**
     * @brief Base64
     * @param str 需要解码的字符串
     * @return: 返回解码后的字符串
     */
    std::string Base64Decode(const std::string_view& src);

    /**
     * @brief Base64
     * @param src 需要编码的字符串
     * @return: 返回编码后的字符串
     */
    std::string Base64Encode(const std::string_view& src);

    /**
     * @brief MD5
     * @param filePath 需要加密的文件
     * @return: 返回文件加密后的十六进制字符串
     */
    std::string FileToHexMD5String(const std::string_view& filePath);
}