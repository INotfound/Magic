#pragma once
#include <string>
#include <openssl/md5.h>
#include <openssl/sha.h>
namespace Magic {
    const char* MD5(const std::string& str);
    const char* SHA1(const std::string& str);
    const char* MD5HexString(const std::string& str);
    const char* SHA1HexString(const std::string& str);
    const char* MD5FileHexString( const char *filename);
    std::string Base64Decode(const std::string &src);
    std::string Base64Encode(const std::string &src);
}