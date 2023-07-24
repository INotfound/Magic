/*
 ******************************************************************************
 * @Author         : INotfound
 * @File           : Crypto.hpp
 * @Date           : 2023-07-03 18:29
 ******************************************************************************
 */
#pragma once
#include <openssl/evp.h>
#include "Magic/Core/Core.hpp"
#include "Magic/Core/Stream.hpp"

namespace Magic{
    class CryptoDecorator :public IStream{
    public:
        explicit CryptoDecorator(const Safe<IStream>& stream);
    protected:
        Safe<IStream> m_Stream;
    };

    class Base64Decoder :public CryptoDecorator{
    public:
        explicit Base64Decoder(const Safe<IStream>& stream);
        BufferView read() override;
        void seek(uint64_t pos) override;
        bool eof() const noexcept override;
        uint64_t size() const noexcept override;
    private:
        void write(const BufferView& data) override;
    private:
        uint64_t m_ChunkLen;
        IStream::BufferType m_Buffer;
    };

    class Base64Encoder :public CryptoDecorator{
    public:
        explicit Base64Encoder(const Safe<IStream>& stream);
        BufferView read() override;
        void seek(uint64_t pos) override;
        bool eof() const noexcept override;
        uint64_t size() const noexcept override;
    private:
        void write(const BufferView& data) override;
    private:
        uint64_t m_ChunkLen;
        IStream::BufferType m_Buffer;
    };

    class MessageDigest :public CryptoDecorator{
    public:
        enum class Algorithm{
            MD4,
            MD5,
            SHA1,
            SHA256,
            SHA384,
            SHA512,
        };
    public:
        explicit MessageDigest(Algorithm algorithm,const Safe<IStream>& stream);
        BufferView read() override;
        void seek(uint64_t pos) override;
    private:
        bool eof() const noexcept override;
        uint64_t size() const noexcept override;
        void write(const BufferView& data) override;
    private:
        uint32_t m_DigestSize;
        std::unique_ptr<EVP_MD_CTX,void (*)(EVP_MD_CTX*)> m_EvpMdCtx;
        std::unique_ptr<IStream::BufferView::value_type,void (*)(IStream::BufferView::value_type*)> m_Digest;
    };
}