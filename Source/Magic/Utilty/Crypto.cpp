/*
 ******************************************************************************
 * @Author         : INotfound
 * @File           : Crypto.cpp
 * @Date           : 2023-07-03 18:34
 ******************************************************************************
 */
#include <openssl/md4.h>
#include <openssl/md5.h>
#include <openssl/sha.h>

#include "Magic/Utilty/Crypto.hpp"

namespace Magic{
    CryptoDecorator::CryptoDecorator(const Safe<IStream>& stream)
        :m_Stream(stream){
        m_Stream->seek(0);
    }

    Base64Decoder::Base64Decoder(const std::shared_ptr<IStream>& stream)
        :CryptoDecorator(stream)
        ,m_ChunkLen(0){
    }

    IStream::BufferView Base64Decoder::read(){
        if(m_Stream->eof()){
            return IStream::BufferView();
        }
        static unsigned char base64DecodeMap[256] ={
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
            0xFF, 0xFF, 0xFF, 0x3E, 0xFF, 0xFF, 0xFF, 0x3F,
            0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0x3A, 0x3B,
            0x3C, 0x3D, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xFF,
            0xFF, 0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06,
            0x07, 0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E,
            0x0F, 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16,
            0x17, 0x18, 0x19, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
            0xFF, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F, 0x20,
            0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27, 0x28,
            0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F, 0x30,
            0x31, 0x32, 0x33, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
        };
        uint8_t group[4];
        m_Buffer.resize(0);
        uint32_t i = 0,j = 0;
        auto bufferView = m_Stream->read();
        m_Buffer.resize(((bufferView.size()+3)/4*3+80));
        for(i = 0; i + 4 <= bufferView.size(); i+=4){
            for(uint32_t k = 0; k < 4; k++){
                group[k] = base64DecodeMap[static_cast<uint8_t>(bufferView[i + k])];
            }
            m_Buffer[j++] = (group[0] << 2) | (group[1] >> 4);
            if(group[2] >= 64){
                break;
            }else if(group[3] >= 64){
                m_Buffer[j++] = (group[1] << 4) | (group[2] >> 2);
            }else{
                m_Buffer[j++] = (group[1] << 4) | (group[2] >> 2);
                m_Buffer[j++] = (group[2] << 6) | (group[3]);
            }
        }
        if(!m_Stream->eof()){
            m_ChunkLen += i;
            m_Stream->seek(m_ChunkLen);
        }
        return IStream::BufferView(m_Buffer.data(),j);
    }

    void Base64Decoder::seek(uint64_t pos){
        m_Stream->seek(pos);
    }

    bool Base64Decoder::eof() const noexcept{
        return m_Stream->eof();
    }

    uint64_t Base64Decoder::size() const noexcept{
        return m_Stream->size();
    }

    void Base64Decoder::write(const IStream::BufferView& data){
        m_Stream->write(data);
    }

    Base64Encoder::Base64Encoder(const std::shared_ptr<IStream>& stream)
        :CryptoDecorator(stream)
        ,m_ChunkLen(0){
    }

    IStream::BufferView Base64Encoder::read(){
        if(m_Stream->eof()){
            return IStream::BufferView();
        }
        static const char* base64EncodeMap = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
        uint32_t i,j;
        m_Buffer.resize(0);
        auto bufferView = m_Stream->read();
        const uint64_t bufferSize = bufferView.size();
        const auto* bufferData = reinterpret_cast<const uint8_t*>(bufferView.data());
        m_Buffer.resize(((bufferSize+2)/3*4)+(bufferSize/48+1)*2+80);
        for(i = 0, j = 0; i+3 <= bufferSize; i+=3){
            m_Buffer[j++] = base64EncodeMap[bufferData[i]>>2];
            m_Buffer[j++] = base64EncodeMap[((bufferData[i]<<4)&0x30)|(bufferData[i+1]>>4)];
            m_Buffer[j++] = base64EncodeMap[((bufferData[i+1]<<2)&0x3c)|(bufferData[i+2]>>6)];
            m_Buffer[j++] = base64EncodeMap[bufferData[i+2]&0x3f];
        }
        if(!m_Stream->eof()){
            m_ChunkLen += i;
            m_Stream->seek(m_ChunkLen);
        }else{
            if(i < bufferSize){
                uint32_t tail = bufferSize - i;
                if(tail == 1){
                    m_Buffer[j++] = base64EncodeMap[bufferData[i] >> 2];
                    m_Buffer[j++] = base64EncodeMap[(bufferData[i] << 4) & 0x30];
                    m_Buffer[j++] = '=';
                    m_Buffer[j++] = '=';
                }else{
                    m_Buffer[j++] = base64EncodeMap[bufferData[i] >> 2];
                    m_Buffer[j++] = base64EncodeMap[((bufferData[i] << 4) & 0x30) | (bufferData[i + 1] >> 4)];
                    m_Buffer[j++] = base64EncodeMap[(bufferData[i + 1] << 2) & 0x3c];
                    m_Buffer[j++] = '=';
                }
            }
        }
        return IStream::BufferView(m_Buffer.data(),j);
    }

    void Base64Encoder::seek(uint64_t pos){
        m_Stream->seek(pos);
    }

    bool Base64Encoder::eof() const noexcept{
        return m_Stream->eof();
    }

    uint64_t Base64Encoder::size() const noexcept{
        return m_Stream->size();
    }

    void Base64Encoder::write(const IStream::BufferView& data){
        m_Stream->write(data);
    }

    MessageDigest::MessageDigest(MessageDigest::Algorithm algorithm,const Safe<IStream>& stream)
        :CryptoDecorator(stream)
        ,m_DigestSize(1)
        ,m_EvpMdCtx(EVP_MD_CTX_new(),[](EVP_MD_CTX* pointer){
            if(pointer){
                EVP_MD_CTX_free(pointer);
            }
        })
        ,m_Digest(nullptr,[](IStream::BufferView::value_type* pointer){
            if(pointer){
                delete[] pointer;
            }
        }){
        switch(algorithm){
            case Algorithm::MD4:
                m_DigestSize = MD4_DIGEST_LENGTH;
                EVP_DigestInit_ex(m_EvpMdCtx.get(),EVP_md4(),nullptr);
                break;
            case Algorithm::MD5:
                m_DigestSize = MD5_DIGEST_LENGTH;
                EVP_DigestInit_ex(m_EvpMdCtx.get(),EVP_md5(),nullptr);
                break;
            case Algorithm::SHA1:
                m_DigestSize = SHA_DIGEST_LENGTH;
                EVP_DigestInit_ex(m_EvpMdCtx.get(),EVP_sha1(),nullptr);
                break;
            case Algorithm::SHA256:
                m_DigestSize = SHA256_DIGEST_LENGTH;
                EVP_DigestInit_ex(m_EvpMdCtx.get(),EVP_sha1(),nullptr);
                EVP_DigestInit_ex(m_EvpMdCtx.get(),EVP_sha256(),nullptr);
                break;
            case Algorithm::SHA384:
                m_DigestSize = SHA384_DIGEST_LENGTH;
                EVP_DigestInit_ex(m_EvpMdCtx.get(),EVP_sha384(),nullptr);
                break;
            case Algorithm::SHA512:
                m_DigestSize = SHA512_DIGEST_LENGTH;
                EVP_DigestInit_ex(m_EvpMdCtx.get(),EVP_sha512(),nullptr);
                break;
        }
        m_Digest.reset(new IStream::BufferView::value_type[m_DigestSize]);
    }

    IStream::BufferView MessageDigest::read(){
        if(m_Stream->eof()){
            return IStream::BufferView();
        }
        do{
            IStream::BufferView bufferView = m_Stream->read();
            EVP_DigestUpdate(m_EvpMdCtx.get(),bufferView.data(),bufferView.size());
        }while(!m_Stream->eof());
        EVP_DigestFinal_ex(m_EvpMdCtx.get(),reinterpret_cast<uint8_t*>(m_Digest.get()),&m_DigestSize);
        return IStream::BufferView(m_Digest.get(),m_DigestSize);
    }

    void MessageDigest::seek(uint64_t pos){
        m_Stream->seek(pos);
    }

    bool MessageDigest::eof() const noexcept{
        return m_Stream->eof();
    }

    uint64_t MessageDigest::size() const noexcept{
        return m_Stream->size();
    }

    void MessageDigest::write(const IStream::BufferView& data){
        m_Stream->write(data);
    }
}
