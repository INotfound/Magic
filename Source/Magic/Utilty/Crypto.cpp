/*
 ******************************************************************************
 * @Author         : INotfound
 * @File           : Crypto.cpp
 * @Date           : 2023-07-03 18:34
 ******************************************************************************
 */
#include <cstdio>
#include <cstring>
#ifdef OPENSSL
#include <openssl/evp.h>
#include <openssl/sha.h>
#include <openssl/md5.h>
#endif
#include "Magic/Utilty/Logger.hpp"
#include "Magic/Utilty/Crypto.hpp"

namespace Magic{
    void HexStringFromData(const void* data,uint32_t len,char* outPut){
        const auto* buf = (const unsigned char*)data;
        size_t i,j;
        for(i = j = 0;i < len;++i){
            char c;
            c = (buf[i] >> 4) & 0xF;
            c = (c > 9) ? c + 'a' - 10 : c + '0';
            outPut[j++] = c;
            c = (buf[i] & 0xF);
            c = (c > 9) ? c + 'a' - 10 : c + '0';
            outPut[j++] = c;
        }
    }

    std::string MD5(const Magic::StringView& str){
        if(str.empty()){
            return std::string();
        }
    #ifdef OPENSSL
        uint32_t len = 0;
        uint8_t digest[MD5_DIGEST_LENGTH] = {0};
        std::unique_ptr<EVP_MD_CTX,void (*)(EVP_MD_CTX*)> evpCtx(EVP_MD_CTX_new(),[](EVP_MD_CTX* pointer){
            if(pointer != nullptr){
                EVP_MD_CTX_free(pointer);
            }
        });
        EVP_DigestInit_ex(evpCtx.get(),EVP_md5(),nullptr);
        EVP_DigestUpdate(evpCtx.get(),str.data(),str.size());
        EVP_DigestFinal_ex(evpCtx.get(),digest,&len);
        return std::string(reinterpret_cast<char*>(digest),len);
    #else
        throw Failure("Requires SSL Support");
    #endif
    }

    std::string SHA1(const Magic::StringView& str){
        if(str.empty()){
            return std::string();
        }
    #ifdef OPENSSL
        uint32_t len = 0;
        uint8_t digest[SHA_DIGEST_LENGTH] = {0};
        std::unique_ptr<EVP_MD_CTX,void (*)(EVP_MD_CTX*)> evpCtx(EVP_MD_CTX_new(),[](EVP_MD_CTX* pointer){
            if(pointer != nullptr){
                EVP_MD_CTX_free(pointer);
            }
        });
        EVP_DigestInit_ex(evpCtx.get(),EVP_sha1(),nullptr);
        EVP_DigestUpdate(evpCtx.get(),str.data(),str.size());
        EVP_DigestFinal_ex(evpCtx.get(),digest,&len);
        return std::string(reinterpret_cast<char*>(digest),len);
    #else
        throw Failure("Requires SSL Support");
    #endif
    }

    std::string StringToHexMD5(const Magic::StringView& str){
        if(str.empty()){
            return std::string();
        }
    #ifdef OPENSSL
        uint32_t len = 0;
        uint8_t digest[MD5_DIGEST_LENGTH] = {0};
        char hexBuffer[MD5_DIGEST_LENGTH*2] = {0};
        std::unique_ptr<EVP_MD_CTX,void (*)(EVP_MD_CTX*)> evpCtx(EVP_MD_CTX_new(),[](EVP_MD_CTX* pointer){
            if(pointer != nullptr){
                EVP_MD_CTX_free(pointer);
            }
        });
        EVP_DigestInit_ex(evpCtx.get(),EVP_md5(),nullptr);
        EVP_DigestUpdate(evpCtx.get(),str.data(),str.size());
        EVP_DigestFinal_ex(evpCtx.get(),digest,&len);
        HexStringFromData(digest,len,hexBuffer);
        return std::string(hexBuffer,len*2);
    #else
        throw Failure("Requires SSL Support");
    #endif
    }

    std::string StringToHexSHA1(const Magic::StringView& str){
        if(str.empty()){
            return std::string();
        }
    #ifdef OPENSSL
        uint32_t len = 0;
        uint8_t digest[SHA_DIGEST_LENGTH] = {0};
        char hexBuffer[SHA_DIGEST_LENGTH*2] = {0};
        std::unique_ptr<EVP_MD_CTX,void (*)(EVP_MD_CTX*)> evpCtx(EVP_MD_CTX_new(),[](EVP_MD_CTX* pointer){
            if(pointer != nullptr){
                EVP_MD_CTX_free(pointer);
            }
        });
        EVP_DigestInit_ex(evpCtx.get(),EVP_sha1(),nullptr);
        EVP_DigestUpdate(evpCtx.get(),str.data(),str.size());
        EVP_DigestFinal_ex(evpCtx.get(),digest,&len);
        HexStringFromData(digest,len,hexBuffer);
        return std::string(hexBuffer,len*2);
    #else
        throw Failure("Requires SSL Support");
    #endif
    }

    std::string Base64Decode(const Magic::StringView& src){
        std::string result;
        result.resize(src.size() * 3 / 4);
        char* writeBuf = &result[0];
        const char* ptr = src.data();
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

        result.resize(writeBuf - result.data());
        return result;
    }

    std::string Base64Encode(const Magic::StringView& src){
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

    std::string FileToHexMD5String(const Magic::StringView& filePath){
        if(filePath.empty()){
            return std::string();
        }
    #ifdef OPENSSL
        std::unique_ptr<std::FILE,void (*)(std::FILE*)> file(std::fopen(filePath.data(),"rb"),[](std::FILE* pointer){
            if(pointer != nullptr){
                std::fclose(pointer);
            }
        });

        if(file == nullptr){
            return std::string();
        }

        uint32_t len = 0;
        uint8_t digest[MD5_DIGEST_LENGTH] = {0};
        char hexBuffer[MD5_DIGEST_LENGTH*2] = {0};
        constexpr const uint32_t bufferSize = 1024*1024;
        std::unique_ptr<uint8_t,void (*)(uint8_t*)> buffer(new uint8_t[bufferSize],[](uint8_t* pointer){
            if(pointer != nullptr){
                delete[] pointer;
            }
        });

        std::unique_ptr<EVP_MD_CTX,void (*)(EVP_MD_CTX*)> evpCtx(EVP_MD_CTX_new(),[](EVP_MD_CTX* pointer){
            if(pointer != nullptr){
                EVP_MD_CTX_free(pointer);
            }
        });

        EVP_DigestInit_ex(evpCtx.get(),EVP_md5(),nullptr);
        while((len = std::fread(buffer.get(),1,bufferSize,file.get()))){
            EVP_DigestUpdate(evpCtx.get(),buffer.get(),len);
        }
        EVP_DigestFinal_ex(evpCtx.get(),digest,&len);
        HexStringFromData(digest,len,hexBuffer);
        return std::string(hexBuffer,len*2);
    #else
        throw Failure("Requires SSL Support");
    #endif
    }
}
