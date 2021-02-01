/*
 * @Author: INotFound
 * @Date: 2020-03-01 15:53:59
 * @LastEditTime: 2021-02-01 22:25:31
 */
#include <cstdio>
#include <cstring>
#ifdef OPENSSL
#include <openssl/md5.h>
#include <openssl/sha.h>
#endif
#include "Utilty/Crypto.h"

namespace Magic{
#ifndef SHA_DIGEST_LENGTH
    #define SHA_DIGEST_LENGTH 256
#endif

    static char MD5Chars[33] = {0};
    static char SHA1Chars[41] = {0};

    void HexStringFromData(const void*data,uint32_t len,char* outPut){
        const unsigned char *buf = (const unsigned char *)data;
        size_t i, j;
        for (i = j = 0; i < len; ++i) {
            char c;
            c = (buf[i] >> 4) & 0xf;
            c = (c > 9) ? c + 'a' - 10 : c + '0';
            outPut[j++] = c;
            c = (buf[i] & 0xf);
            c = (c > 9) ? c + 'a' - 10 : c + '0';
            outPut[j++] = c;
        }
    }

    const char* MD5(const std::string& str){
        if(str.empty()){
            return nullptr;
        }
        static unsigned char digest[SHA_DIGEST_LENGTH] = {0};
    #ifdef OPENSSL
        std::memset(MD5Chars,0,33);
        MD5_CTX m_Md5;
        MD5_Init(&m_Md5);
        MD5_Update(&m_Md5,str.data(), str.size());
        MD5_Final(digest,&m_Md5);
    #endif
        return reinterpret_cast<char*>(digest);
    }

    const char* SHA1(const std::string& str){
        if(str.empty()){
            return nullptr;
        }
        static unsigned char digest[SHA_DIGEST_LENGTH] = {0};
    #ifdef OPENSSL
        std::memset(SHA1Chars,0,41);
        SHA_CTX m_Sha1;
        SHA1_Init(&m_Sha1);
        SHA1_Update(&m_Sha1,str.data(), str.size());
        SHA1_Final(digest,&m_Sha1);
    #endif
        return reinterpret_cast<char*>(digest);
    }

    const char* MD5HexString(const std::string& str) {
        if(str.empty()){
            return nullptr;
        }
    #ifdef OPENSSL
        std::memset(MD5Chars,0,33);
        MD5_CTX m_Md5;
        MD5_Init(&m_Md5);
	    static unsigned char digest[MD5_DIGEST_LENGTH] = {0};
        MD5_Update(&m_Md5,str.data(), str.size());
        MD5_Final(digest,&m_Md5);
        HexStringFromData(digest,MD5_DIGEST_LENGTH,MD5Chars);
    #endif
        return MD5Chars;
    }

    const char* SHA1HexString(const std::string& str){
        if(str.empty()){
            return nullptr;
        }
    #ifdef OPENSSL
        std::memset(SHA1Chars,0,41);
        SHA_CTX m_Sha1;
        SHA1_Init(&m_Sha1);
	    static unsigned char digest[SHA_DIGEST_LENGTH] = {0};
        SHA1_Update(&m_Sha1,str.data(), str.size());
        SHA1_Final(digest,&m_Sha1);
        HexStringFromData(digest,SHA_DIGEST_LENGTH,SHA1Chars);
    #endif
        return SHA1Chars;
    }

    const char* MD5FileHexString(const char *filename) {
        if (nullptr == filename || std::strcmp(filename, "") == 0)
            return nullptr;
    #ifdef OPENSSL
        std::FILE *file;
        static unsigned char buffer[1024] = {0};
	    static unsigned char digest[MD5_DIGEST_LENGTH] = {0};
        if((file = std::fopen(filename, "rb")) == nullptr) {
            return nullptr;
        }
        int len = {0};
        std::memset(MD5Chars,0,33);
        MD5_CTX m_Md5;
        MD5_Init(&m_Md5);
        while((len = std::fread(buffer, 1, 1024, file)))
            MD5_Update(&m_Md5,buffer, len);
        MD5_Final(digest,&m_Md5);
        std::fclose(file);
        HexStringFromData(digest,MD5_DIGEST_LENGTH,MD5Chars);
    #endif
        return MD5Chars;
    }
    
    std::string Base64Decode(const std::string &src) {
        std::string result;
        result.resize(src.size() * 3 / 4);
        char *writeBuf = &result[0];
        const char* ptr = src.c_str();
        const char* end = ptr + src.size();
        while(ptr < end) {
            int i = 0;
            int padding = 0;
            int packed = 0;
            for(; i < 4 && ptr < end; ++i, ++ptr) {
                if(*ptr == '=') {
                    ++padding;
                    packed <<= 6;
                    continue;
                }
                // padding with "=" only
                if (padding > 0) {
                    return "";
                }

                int val = 0;
                if(*ptr >= 'A' && *ptr <= 'Z') {
                    val = *ptr - 'A';
                } else if(*ptr >= 'a' && *ptr <= 'z') {
                    val = *ptr - 'a' + 26;
                } else if(*ptr >= '0' && *ptr <= '9') {
                    val = *ptr - '0' + 52;
                } else if(*ptr == '+') {
                    val = 62;
                } else if(*ptr == '/') {
                    val = 63;
                } else {
                    return ""; // invalid character
                }

                packed = (packed << 6) | val;
            }
            if (i != 4) {
                return "";
            }
            if (padding > 0 && ptr != end) {
                return "";
            }
            if (padding > 2) {
                return "";
            }

            *writeBuf++ = (char)((packed >> 16) & 0xff);
            if(padding != 2) {
                *writeBuf++ = (char)((packed >> 8) & 0xff);
            }
            if(padding == 0) {
                *writeBuf++ = (char)(packed & 0xff);
            }
        }

        result.resize(writeBuf - result.c_str());
        return result;
    }
    
    std::string Base64Encode(const std::string &src){
        std::string ret;
        uint64_t len = src.size();
        const char* data = src.data();
        static const char* base64 =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
        ret.reserve(len * 4 / 3 + 2);
        const unsigned char* ptr = (const unsigned char*)data;
        const unsigned char* end = ptr + len;
        while(ptr < end) {
            unsigned int packed = 0;
            int i = 0;
            int padding = 0;
            for(; i < 3 && ptr < end; ++i, ++ptr) {
                packed = (packed << 8) | *ptr;
            }
            if(i == 2) {
                padding = 1;
            } else if (i == 1) {
                padding = 2;
            }
            for(; i < 3; ++i) {
                packed <<= 8;
            }

            ret.append(1, base64[packed >> 18]);
            ret.append(1, base64[(packed >> 12) & 0x3f]);
            if(padding != 2) {
                ret.append(1, base64[(packed >> 6) & 0x3f]);
            }
            if(padding == 0) {
                ret.append(1, base64[packed & 0x3f]);
            }
            ret.append(padding, '=');
        }
        return ret;
    }
}