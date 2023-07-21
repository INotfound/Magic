#include "gtest/gtest.h"
#include <memory>
#include "Magic/Core/Stream.hpp"
#include "Magic/Utilty/Compress.hpp"

TEST(MagicCompressTest,DataStreamBrotliEncoderAndBrotliDecoder){
    std::string data;
    std::string compressData;
    std::string unCompressData;
    for(auto i = 0;i < 1000;i++){
        data.append("AASDFGHJKSDFGHJKDFGHJKCVBNRGHJK");
    }

    {
        auto compressStream = std::make_shared<Magic::DataStream>(data);
        auto brotliEncoder = std::make_shared<Magic::BrotliEncoder>(compressStream);
        while(!brotliEncoder->eof()){
            auto bufferView = brotliEncoder->read();
            compressData.append(bufferView.data(),bufferView.size());
        }
    }

    {
        auto unCompressStream = std::make_shared<Magic::DataStream>(compressData);
        auto brotliDecoder = std::make_shared<Magic::BrotliDecoder>(unCompressStream);
        while(!brotliDecoder->eof()){
            auto bufferView = brotliDecoder->read();
            unCompressData.append(bufferView.begin(),bufferView.end());
        }
        EXPECT_EQ(unCompressData.size(),data.size());
        EXPECT_STREQ(std::string(unCompressData.data(),unCompressData.size()).data(),data.data());
    }
}

TEST(MagicCompressTest,FileStreamBrotliEncoderAndBrotliDecoder){
    std::string data;
    Magic::StringView br = "Brotli.br";
    Magic::StringView txt = "Brotli.txt";
    {
        for(auto i = 0;i < 1000;i++){
            data.append("XXXX");
        }
        Magic::FileStream fileStream(txt);
        fileStream.open(Magic::FileStream::OpenMode::Write);
        fileStream.write(data);
    }

    {
        std::string compressData;
        auto compressStream = std::make_shared<Magic::FileStream>(txt);
        EXPECT_EQ(compressStream->open(Magic::FileStream::OpenMode::Read),true);
        auto brotliEncoder = std::make_shared<Magic::BrotliEncoder>(compressStream);
        auto saveFileStream = std::make_shared<Magic::FileStream>(br);
        EXPECT_EQ(saveFileStream->open(Magic::FileStream::OpenMode::Write),true);
        while(!brotliEncoder->eof()){
            auto bufferView = brotliEncoder->read();
            saveFileStream->write(bufferView);
            compressData.append(bufferView.data(),bufferView.size());
        }
    }

    {
        std::string unCompressData;
        auto unCompressStream = std::make_shared<Magic::FileStream>(br);
        EXPECT_EQ(unCompressStream->open(Magic::FileStream::OpenMode::Read),true);
        auto brotliDecoder = std::make_shared<Magic::BrotliDecoder>(unCompressStream);
        while(!brotliDecoder->eof()){
            auto bufferView = brotliDecoder->read();
            unCompressData.append(bufferView.data(),bufferView.size());
        }
        EXPECT_EQ(unCompressData.size(),data.size());
        EXPECT_STREQ(unCompressData.data(),data.data());
    }
}
#ifdef ZLIB
TEST(MagicCompressTest,GZipEncoderAndBrotliDecoder){
    std::string data;
    std::string compressData;
    std::string unCompressData;
    for(auto i = 0;i < 1000;i++){
        data.append("AASDFGHJKSDFGHJKDFGHJKCVBNRGHJK");
    }

    {
        auto compressStream = std::make_shared<Magic::DataStream>(data);
        auto zipEncoder = std::make_shared<Magic::GZipEncoder>(compressStream);
        while(!zipEncoder->eof()){
            auto bufferView = zipEncoder->read();
            compressData.append(bufferView.data(),bufferView.size());
        }
    }

    {
        auto unCompressStream = std::make_shared<Magic::DataStream>(compressData);
        auto zipDecoder = std::make_shared<Magic::GZipDecoder>(unCompressStream);
        while(!zipDecoder->eof()){
            auto bufferView = zipDecoder->read();
            unCompressData.append(bufferView.begin(),bufferView.end());
        }
        EXPECT_EQ(unCompressData.size(),data.size());
        EXPECT_STREQ(std::string(unCompressData.data(),unCompressData.size()).data(),data.data());
    }
}

TEST(MagicCompressTest,FileStreamGZipEncoderAndBrotliDecoder){
    std::string data;
    Magic::StringView br = "GZip.br";
    Magic::StringView txt = "GZip.txt";
    {
        for(auto i = 0;i < 1000;i++){
            data.append("XXXX");
        }
        Magic::FileStream fileStream(txt);
        fileStream.open(Magic::FileStream::OpenMode::Write);
        fileStream.write(data);
    }

    {
        std::string compressData;
        auto compressStream = std::make_shared<Magic::FileStream>(txt);
        EXPECT_EQ(compressStream->open(Magic::FileStream::OpenMode::Read),true);
        auto zipEncoder = std::make_shared<Magic::GZipEncoder>(compressStream);
        auto saveFileStream = std::make_shared<Magic::FileStream>(br);
        EXPECT_EQ(saveFileStream->open(Magic::FileStream::OpenMode::Write),true);
        while(!zipEncoder->eof()){
            auto bufferView = zipEncoder->read();
            saveFileStream->write(bufferView);
            compressData.append(bufferView.data(),bufferView.size());
        }
    }

    {
        std::string unCompressData;
        auto unCompressStream = std::make_shared<Magic::FileStream>(br);
        EXPECT_EQ(unCompressStream->open(Magic::FileStream::OpenMode::Read),true);
        auto zipDecoder = std::make_shared<Magic::GZipDecoder>(unCompressStream);
        while(!zipDecoder->eof()){
            auto bufferView = zipDecoder->read();
            unCompressData.append(bufferView.data(),bufferView.size());
        }
        EXPECT_EQ(unCompressData.size(),data.size());
        EXPECT_STREQ(unCompressData.data(),data.data());
    }
}
#endif