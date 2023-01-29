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
#include "Magic/Utilty/Logger.h"
#include "Magic/Utilty/Crypto.h"

namespace Magic{
    void HexStringFromData(const void* data,uint32_t len,char* outPut){
        const auto* buf = (const unsigned char*)data;
        size_t i,j;
        for(i = j = 0;i < len;++i){
            char c;
            c = (buf[i] >> 4) & 0xf;
            c = (c > 9) ? c + 'a' - 10 : c + '0';
            outPut[j++] = c;
            c = (buf[i] & 0xf);
            c = (c > 9) ? c + 'a' - 10 : c + '0';
            outPut[j++] = c;
        }
    }

    std::string MD5(const std::string& str){
        if(str.empty()){
            return std::string();
        }
        uint8_t digest[MD5_DIGEST_LENGTH + 1] = {0};
    #ifdef OPENSSL
        MD5_CTX m_Md5;
        MD5_Init(&m_Md5);
        MD5_Update(&m_Md5,str.data(),str.size());
        MD5_Final(digest,&m_Md5);
    #else
        MAGIC_ERROR() << "Requires SSL Support.";
    #endif
        return std::string(reinterpret_cast<char*>(digest),MD5_DIGEST_LENGTH + 1);
    }

    std::string SHA1(const std::string& str){
        if(str.empty()){
            return std::string();
        }
        uint8_t digest[SHA_DIGEST_LENGTH + 1] = {0};
    #ifdef OPENSSL
        SHA_CTX m_Sha1;
        SHA1_Init(&m_Sha1);
        SHA1_Update(&m_Sha1,str.data(),str.size());
        SHA1_Final(digest,&m_Sha1);
    #else
        MAGIC_ERROR() << "Requires SSL Support.";
    #endif
        return std::string(reinterpret_cast<char*>(digest),SHA_DIGEST_LENGTH + 1);
    }

    std::string StringToHexMD5(const std::string& str){
        if(str.empty()){
            return std::string();
        }
        char hexBuffer[33] = {0};
    #ifdef OPENSSL
        MD5_CTX m_Md5;
        MD5_Init(&m_Md5);
        uint8_t digest[MD5_DIGEST_LENGTH] = {0};
        MD5_Update(&m_Md5,str.data(),str.size());
        MD5_Final(digest,&m_Md5);
        HexStringFromData(digest,MD5_DIGEST_LENGTH,hexBuffer);
    #else
        MAGIC_ERROR() << "Requires SSL Support.";
    #endif
        return std::string(hexBuffer,33);
    }

    std::string StringToHexSHA1(const std::string& str){
        if(str.empty()){
            return std::string();
        }
        char hexBuffer[41] = {0};
    #ifdef OPENSSL
        SHA_CTX m_Sha1;
        SHA1_Init(&m_Sha1);
        uint8_t digest[SHA_DIGEST_LENGTH] = {0};
        SHA1_Update(&m_Sha1,str.data(),str.size());
        SHA1_Final(digest,&m_Sha1);
        HexStringFromData(digest,SHA_DIGEST_LENGTH,hexBuffer);
    #else
        MAGIC_ERROR() << "Requires SSL Support.";
    #endif
        return std::string(hexBuffer,41);
    }

    std::string Base64Decode(const std::string& src){
        std::string result;
        result.resize(src.size() * 3 / 4);
        char* writeBuf = &result[0];
        const char* ptr = src.c_str();
        const char* end = ptr + src.size();
        while(ptr < end){
            int i = 0;
            int padding = 0;
            int packed = 0;
            for(;i < 4 && ptr < end;++i,++ptr){
                if(*ptr == '='){
                    ++padding;
                    packed <<= 6;
                    continue;
                }
                // padding with "=" only
                if(padding > 0){
                    return "";
                }

                int val = 0;
                if(*ptr >= 'A' && *ptr <= 'Z'){
                    val = *ptr - 'A';
                }else if(*ptr >= 'a' && *ptr <= 'z'){
                    val = *ptr - 'a' + 26;
                }else if(*ptr >= '0' && *ptr <= '9'){
                    val = *ptr - '0' + 52;
                }else if(*ptr == '+'){
                    val = 62;
                }else if(*ptr == '/'){
                    val = 63;
                }else{
                    return ""; // invalid character
                }

                packed = (packed << 6) | val;
            }
            if(i != 4){
                return "";
            }
            if(padding > 0 && ptr != end){
                return "";
            }
            if(padding > 2){
                return "";
            }

            *writeBuf++ = (char)((packed >> 16) & 0xff);
            if(padding != 2){
                *writeBuf++ = (char)((packed >> 8) & 0xff);
            }
            if(padding == 0){
                *writeBuf++ = (char)(packed & 0xff);
            }
        }

        result.resize(writeBuf - result.c_str());
        return result;
    }

    std::string Base64Encode(const std::string& src){
        std::string ret;
        uint64_t len = src.size();
        const char* data = src.data();
        static const char* base64 = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
        ret.reserve(len * 4 / 3 + 2);
        const auto* ptr = (const unsigned char*)data;
        const unsigned char* end = ptr + len;
        while(ptr < end){
            unsigned int packed = 0;
            int i = 0;
            int padding = 0;
            for(;i < 3 && ptr < end;++i,++ptr){
                packed = (packed << 8) | *ptr;
            }
            if(i == 2){
                padding = 1;
            }else if(i == 1){
                padding = 2;
            }
            for(;i < 3;++i){
                packed <<= 8;
            }

            ret.append(1,base64[packed >> 18]);
            ret.append(1,base64[(packed >> 12) & 0x3f]);
            if(padding != 2){
                ret.append(1,base64[(packed >> 6) & 0x3f]);
            }
            if(padding == 0){
                ret.append(1,base64[packed & 0x3f]);
            }
            ret.append(padding,'=');
        }
        return ret;
    }

    std::string FileToHexMD5String(const std::string& filePath){
        if(filePath.empty()){
            return std::string();
        }
        #ifdef OPENSSL

        std::unique_ptr<std::FILE,void (*)(std::FILE*)> file(std::fopen(filePath.c_str(),"rb"),[](std::FILE* pointer){
            if(pointer != nullptr){
                std::fclose(pointer);
            }
        });

        if(file == nullptr){
            return std::string();
        }

        uint8_t digest[MD5_DIGEST_LENGTH] = {0};
        constexpr const uint32_t bufferSize = 1024*1024;

        std::unique_ptr<uint8_t,void (*)(uint8_t*)> buffer(new uint8_t[bufferSize],[](uint8_t* pointer){
            if(pointer != nullptr){
                delete[] pointer;
            }
        });

        uint64_t len = 0;
        char hexBuffer[33] = {0};

        MD5_CTX m_Md5;
        MD5_Init(&m_Md5);
        while((len = std::fread(buffer.get(),1,bufferSize,file.get())))
            MD5_Update(&m_Md5,buffer.get(),len);
        MD5_Final(digest,&m_Md5);

        HexStringFromData(digest,MD5_DIGEST_LENGTH,hexBuffer);
        #else
        MAGIC_ERROR() << "Requires SSL Support.";
        #endif
        return std::string(hexBuffer,33);
    }
}
