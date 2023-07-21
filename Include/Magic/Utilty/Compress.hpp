/*
 ******************************************************************************
 * @Author         : INotfound
 * @File           : Compress.hpp
 * @Date           : 2023-07-03 18:29
 ******************************************************************************
 */
#pragma once
#ifdef ZLIB
#include <zlib.h>
#endif
#include <brotli/encode.h>
#include <brotli/decode.h>

#include "Magic/Core/Core.hpp"
#include "Magic/Core/Stream.hpp"

namespace Magic{
    class CompressionDecorator :public IStream{
    public:
        explicit CompressionDecorator(const Safe<IStream>& stream);
    protected:
        Safe<IStream> m_Stream;
    };
#ifdef ZLIB

    class GZipDecoder :public CompressionDecorator{
    public:
        explicit GZipDecoder(const Safe<IStream>& stream);
        BufferView read() override;
        void seek(uint64_t pos) override;
        bool eof() const noexcept override;
        uint64_t size() const noexcept override;
    private:
        void write(const BufferView& data) override;
    private:
        z_stream m_ZStream;
        BufferType m_Buffer;
    };

    class GZipEncoder :public CompressionDecorator{
    public:
        explicit GZipEncoder(const Safe<IStream>& stream);
        BufferView read() override;
        void seek(uint64_t pos) override;
        bool eof() const noexcept override;
        uint64_t size() const noexcept override;
    private:
        void write(const BufferView& data) override;
    private:
        z_stream m_ZStream;
        BufferType m_Buffer;
    };
#endif

    class BrotliDecoder :public CompressionDecorator{
    public:
        explicit BrotliDecoder(const Safe<IStream>& stream);
        BufferView read() override;
        void seek(uint64_t pos) override;
        bool eof() const noexcept override;
        uint64_t size() const noexcept override;
    private:
        void write(const BufferView& data) override;
    private:
        BufferType m_Buffer;
        std::unique_ptr<BrotliDecoderState,void (*)(BrotliDecoderState*)> m_BrotliDecoderState;
    };

    class BrotliEncoder :public CompressionDecorator{
    public:
        explicit BrotliEncoder(const Safe<IStream>& stream);
        BufferView read() override;
        void seek(uint64_t pos) override;
        bool eof() const noexcept override;
        uint64_t size() const noexcept override;
    private:
        void write(const BufferView& data) override;
    private:
        BufferType m_Buffer;
        std::unique_ptr<BrotliEncoderState,void (*)(BrotliEncoderState*)> m_BrotliEncoderState;
    };


}
