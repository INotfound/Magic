#ifdef ZLIB
#include <zlib.h>
#endif
#include <memory>
#include <brotli/encode.h>
#include <brotli/decode.h>
#include "Magic/Utilty/Compress.h"

namespace Magic{
#ifdef ZLIB
    //from https://github.com/chafey/GZipCodec
    #define CHUNK 16384
    #define windowBits 15
    #define GZIP_ENCODING 16

    bool GZipDecoder(const std::string& compressedData,std::string& data){
        int ret;
        unsigned have;
        z_stream strm;
        unsigned char out[CHUNK];

        strm.zalloc = Z_NULL;
        strm.zfree = Z_NULL;
        strm.opaque = Z_NULL;
        strm.avail_in = 0;
        strm.next_in = Z_NULL;
        if(inflateInit2(&strm,16 + MAX_WBITS) != Z_OK){
            return false;
        }

        strm.avail_in = (uInt)compressedData.length();
        strm.next_in = (unsigned char*)compressedData.data();
        do{
            strm.avail_out = CHUNK;
            strm.next_out = out;
            ret = inflate(&strm,Z_NO_FLUSH);
            switch(ret){
                case Z_NEED_DICT:
                case Z_DATA_ERROR:
                case Z_MEM_ERROR:
                    inflateEnd(&strm);
                    return false;
            }
            have = CHUNK - strm.avail_out;
            data.append((char*)out,have);
        }while(strm.avail_out == 0);

        if(inflateEnd(&strm) != Z_OK){
            return false;
        }

        return true;
    }

    bool GZipEncoder(const std::string& data,std::string& compressedData){
        unsigned char out[CHUNK];
        z_stream strm;
        strm.zalloc = Z_NULL;
        strm.zfree = Z_NULL;
        strm.opaque = Z_NULL;
        if(deflateInit2(&strm,-1,Z_DEFLATED,windowBits | GZIP_ENCODING,8,Z_DEFAULT_STRATEGY) != Z_OK){
            return false;
        }
        strm.next_in = (unsigned char*)data.data();
        strm.avail_in = (uInt)data.length();
        do{
            int have;
            strm.avail_out = CHUNK;
            strm.next_out = out;
            if(deflate(&strm,Z_FINISH) == Z_STREAM_ERROR){
                return false;
            }
            have = CHUNK - strm.avail_out;
            compressedData.append((char*)out,have);
        }while(strm.avail_out == 0);
        if(deflateEnd(&strm) != Z_OK){
            return false;
        }
        return true;
    }
#endif

    bool BrotliDecoder(const std::string& compressedData,std::string& data){
        BrotliDecoderResult result;
        std::array<uint8_t, 2048> buffer;
        std::unique_ptr<BrotliDecoderState,void (*)(BrotliDecoderState*)> instance(BrotliDecoderCreateInstance(nullptr,nullptr,nullptr),[](BrotliDecoderState* pointer){
            if(pointer)
                BrotliDecoderDestroyInstance(pointer);
        });
        size_t available_in = compressedData.length(), available_out = buffer.size();
        const auto *next_in = reinterpret_cast<const uint8_t *>(compressedData.c_str());
        uint8_t *next_out = buffer.data();
        do{
            result = BrotliDecoderDecompressStream(
                instance.get(),
                &available_in, &next_in, &available_out, &next_out, nullptr);
            data.append(reinterpret_cast<const char *>(buffer.data()), buffer.size() - available_out);
            available_out = buffer.size();
            next_out = buffer.data();
        } while (!(available_in == 0 && result == BROTLI_DECODER_RESULT_SUCCESS));

        return result == BROTLI_DECODER_RESULT_SUCCESS;
    }

    bool BrotliEncoder(const std::string& data,std::string& compressedData){
        std::array<uint8_t, 2048> buffer;
        std::unique_ptr<BrotliEncoderState,void (*)(BrotliEncoderState*)> instance(BrotliEncoderCreateInstance(nullptr,nullptr,nullptr),[](BrotliEncoderState* pointer){
            if(pointer)
                BrotliEncoderDestroyInstance(pointer);
        });
        BrotliEncoderSetParameter(instance.get(),BrotliEncoderParameter::BROTLI_PARAM_QUALITY,BROTLI_MIN_QUALITY);
        size_t available_in = data.length(), available_out = buffer.size();
        const auto* next_in = reinterpret_cast<const unsigned char *>(data.c_str());
        uint8_t *next_out = buffer.data();
        bool isFinished = false;
        do{
            BrotliEncoderCompressStream(
                instance.get(), BROTLI_OPERATION_FINISH,
                &available_in, &next_in, &available_out, &next_out, nullptr);
            compressedData.append(reinterpret_cast<const char *>(buffer.data()), buffer.size() - available_out);
            available_out = buffer.size();
            next_out = buffer.data();
            isFinished = BrotliEncoderIsFinished(instance.get());
        } while (!(available_in == 0 && isFinished));
        return isFinished;
    }
}
