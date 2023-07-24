/*
 ******************************************************************************
 * @Author         : INotfound
 * @File           : Compress.cpp
 * @Date           : 2023-07-03 18:33
 ******************************************************************************
 */
#include "Magic/Utilty/Compress.hpp"

namespace Magic{
    constexpr uint32_t g_BufferSize = 1024;

    CompressionDecorator::CompressionDecorator(const Safe<IStream>& stream)
        :m_Stream(stream){
    }

#ifdef ZLIB
    GZipDecoder::GZipDecoder(const Safe<IStream>& stream)
        :CompressionDecorator(stream){
        m_ZStream.zfree = Z_NULL;
        m_ZStream.zalloc = Z_NULL;
        m_ZStream.opaque = Z_NULL;
        m_ZStream.next_in = Z_NULL;
        m_ZStream.avail_in = Z_NULL;
        inflateInit2(&m_ZStream,16 + MAX_WBITS);
    }

    IStream::BufferView GZipDecoder::read(){
        if(m_Stream->eof()){
            return IStream::BufferView();
        }
        m_Buffer.resize(0);
        IStream::BufferView bufferView = m_Stream->read();
        m_ZStream.avail_in = bufferView.size();
        std::array<uint8_t,g_BufferSize> buffer;
        m_ZStream.next_in = reinterpret_cast<uint8_t*>(const_cast<IStream::BufferView::pointer>(bufferView.data()));

        do{
            m_ZStream.next_out = buffer.data();
            m_ZStream.avail_out = g_BufferSize;
            auto ret = inflate(&m_ZStream,Z_NO_FLUSH);
            if(ret == Z_NEED_DICT || ret == Z_MEM_ERROR || ret == Z_DATA_ERROR){
                inflateEnd(&m_ZStream);
            }
            m_Buffer.insert(m_Buffer.end(),buffer.data(),buffer.data() + (g_BufferSize - m_ZStream.avail_out));
        }while(m_ZStream.avail_out == 0);

        if(m_Stream->eof()){
            inflateEnd(&m_ZStream);
        }
        return IStream::BufferView(m_Buffer.data(),m_Buffer.size());
    }

    void GZipDecoder::seek(uint64_t pos){
        m_Stream->seek(pos);
    }

    bool GZipDecoder::eof() const noexcept{
        return m_Stream->eof();
    }

    uint64_t GZipDecoder::size() const noexcept{
        return m_Stream->size();
    }

    void GZipDecoder::write(const IStream::BufferView& data){
    }

    GZipEncoder::GZipEncoder(const Safe<IStream>& stream)
        :CompressionDecorator(stream){
        m_ZStream.zfree = Z_NULL;
        m_ZStream.zalloc = Z_NULL;
        m_ZStream.opaque = Z_NULL;
        deflateInit2(&m_ZStream,-1,Z_DEFLATED,15|16,8,Z_DEFAULT_STRATEGY);
    }

    IStream::BufferView GZipEncoder::read(){
        if(m_Stream->eof()){
            return IStream::BufferView();
        }
        m_Buffer.resize(0);
        IStream::BufferView bufferView = m_Stream->read();
        m_ZStream.avail_in = bufferView.size();
        std::array<uint8_t,g_BufferSize> buffer;
        m_ZStream.next_in = reinterpret_cast<uint8_t*>(const_cast<IStream::BufferView::pointer>(bufferView.data()));
        int32_t ret = 0;
        do{
            m_ZStream.next_out = buffer.data();
            m_ZStream.avail_out = g_BufferSize;
            ret = deflate(&m_ZStream,Z_FINISH);
            m_Buffer.insert(m_Buffer.end(),buffer.data(),buffer.data() + (g_BufferSize - m_ZStream.avail_out));
        }while(ret == Z_OK);
        if(m_Stream->eof()){
            inflateEnd(&m_ZStream);
        }
        return IStream::BufferView(m_Buffer.data(),m_Buffer.size());
    }

    void GZipEncoder::seek(uint64_t pos){
        m_Stream->seek(pos);
    }

    bool GZipEncoder::eof() const noexcept{
        return m_Stream->eof();
    }

    uint64_t GZipEncoder::size() const noexcept{
        return m_Stream->size();
    }

    void GZipEncoder::write(const IStream::BufferView& data){
    }

#endif

    BrotliDecoder::BrotliDecoder(const Safe<IStream>& stream)
        :CompressionDecorator(stream)
        ,m_BrotliDecoderState(BrotliDecoderCreateInstance(nullptr,nullptr,nullptr),[](BrotliDecoderState* pointer){
            if(pointer){
                BrotliDecoderDestroyInstance(pointer);
            }
        }){
    }

    IStream::BufferView BrotliDecoder::read(){
        if(m_Stream->eof()){
            return IStream::BufferView();
        }
        m_Buffer.resize(0);
        auto bufferView = m_Stream->read();
        std::array<uint8_t,g_BufferSize> buffer;
        size_t available_in = bufferView.size();
        const auto* next_in = reinterpret_cast<const uint8_t*>(bufferView.data());
        BrotliDecoderResult result = BROTLI_DECODER_RESULT_NEEDS_MORE_INPUT;
        do{
            auto* next_out = buffer.data();
            size_t available_out = g_BufferSize;
            result = BrotliDecoderDecompressStream(m_BrotliDecoderState.get(),
                                                     &available_in, &next_in, &available_out, &next_out, nullptr);
            if(available_out != g_BufferSize && result != BROTLI_DECODER_RESULT_ERROR){
                m_Buffer.insert(m_Buffer.end(),buffer.data(),buffer.data()+(g_BufferSize - available_out));
            }
        }while(result == BROTLI_DECODER_RESULT_NEEDS_MORE_OUTPUT);
        return IStream::BufferView(m_Buffer.data(),m_Buffer.size());
    }

    void BrotliDecoder::seek(uint64_t pos){
        m_Stream->seek(pos);
    }

    bool BrotliDecoder::eof() const noexcept{
        return m_Stream->eof();
    }

    uint64_t BrotliDecoder::size() const noexcept{
        return m_Stream->size();
    }

    void BrotliDecoder::write(const IStream::BufferView& data){
    }

    BrotliEncoder::BrotliEncoder(const Safe<IStream>& stream)
        :CompressionDecorator(stream)
        ,m_BrotliEncoderState(BrotliEncoderCreateInstance(nullptr,nullptr,nullptr),[](BrotliEncoderState* pointer){
            if(pointer){
                BrotliEncoderDestroyInstance(pointer);
            }
        }){
    }

    IStream::BufferView BrotliEncoder::read(){
        if(m_Stream->eof()){
            return IStream::BufferView();
        }
        m_Buffer.resize(0);
        auto bufferView = m_Stream->read();
        std::array<uint8_t,g_BufferSize> buffer;
        size_t available_in = bufferView.size();
        const auto* next_in = reinterpret_cast<const uint8_t*>(bufferView.data());
        BrotliEncoderSetParameter(m_BrotliEncoderState.get(),BrotliEncoderParameter::BROTLI_PARAM_QUALITY,BROTLI_MIN_QUALITY);
        BrotliEncoderOperation operation = BROTLI_OPERATION_PROCESS;
        do{
            if(m_Stream->eof()){
                operation = BROTLI_OPERATION_FINISH;
            }
            uint8_t* next_out = buffer.data();
            size_t available_out = g_BufferSize;
            BrotliEncoderCompressStream(m_BrotliEncoderState.get(), operation,
                                        &available_in, &next_in, &available_out, &next_out, nullptr);
            m_Buffer.insert(m_Buffer.end(),buffer.data(),buffer.data()+(g_BufferSize - available_out));
        }while(available_in > 0 || (operation == BROTLI_OPERATION_FINISH && (BrotliEncoderIsFinished(m_BrotliEncoderState.get()) != BROTLI_TRUE)));
        return IStream::BufferView(m_Buffer.data(),m_Buffer.size());
    }

    void BrotliEncoder::seek(uint64_t pos){
        m_Stream->seek(pos);
    }

    bool BrotliEncoder::eof() const noexcept{
        return m_Stream->eof();
    }

    uint64_t BrotliEncoder::size() const noexcept{
        return m_Stream->size();
    }

    void BrotliEncoder::write(const IStream::BufferView& data){
    }
}
