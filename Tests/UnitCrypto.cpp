#include "gtest/gtest.h"
#include "Magic/Core/Stream.hpp"
#include "Magic/Utilty/Crypto.hpp"
#include "Magic/Utilty/String.hpp"

TEST(MagicCryptoTest,StringToHexMD5){
    auto stream = std::make_shared<Magic::MessageDigest>(Magic::MessageDigest::Algorithm::MD5,std::make_shared<Magic::DataStream>("Hello World"));
    EXPECT_STREQ(Magic::ToHexString(stream->read()).data(),"b10a8db164e0754105b7a99be72e3fe5");
}

TEST(MagicCryptoTest,StringToHexSHA1){
    auto stream = std::make_shared<Magic::MessageDigest>(Magic::MessageDigest::Algorithm::SHA1,std::make_shared<Magic::DataStream>("Hello World"));
    EXPECT_STREQ(Magic::ToHexString(stream->read()).data(),"0a4d55a8d778e5022fab701977c5d840bbc486d0");
}

TEST(MagicCryptoTest,FileToHexMD5String){
    {
        auto fileStream = std::make_shared<Magic::FileStream>("CryptoTestMd5.txt");
        EXPECT_EQ(fileStream->open(Magic::FileStream::OpenMode::Write),true);
        fileStream->write("Hello World");
    }

    {
        auto fileStream = std::make_shared<Magic::FileStream>("CryptoTestMd5.txt");
        EXPECT_EQ(fileStream->open(Magic::FileStream::OpenMode::Read),true);
        auto stream = std::make_shared<Magic::MessageDigest>(Magic::MessageDigest::Algorithm::MD5,fileStream);
        EXPECT_STREQ(Magic::ToHexString(stream->read()).data(),"b10a8db164e0754105b7a99be72e3fe5");
    }
}

TEST(MagicCryptoTest,Base64EncoderAndDecoder){
    {
        std::string dataString;
        auto dataStream = std::make_shared<Magic::DataStream>("SGVsbG9Xb3JsZA==");
        Safe<Magic::Base64Decoder> decoder = std::make_shared<Magic::Base64Decoder>(dataStream);
        while(!decoder->eof()){
            auto bufferView = decoder->read();
            dataString.append(bufferView.data(),bufferView.size());
        }
        EXPECT_STREQ(dataString.data(),"HelloWorld");
    }
    {
        std::string dataString;
        auto dataStream = std::make_shared<Magic::DataStream>("HelloWorld");
        Safe<Magic::Base64Encoder> encoder = std::make_shared<Magic::Base64Encoder>(dataStream);
        while(!encoder->eof()){
            auto bufferView = encoder->read();
            dataString.append(bufferView.data(),bufferView.size());
        }
        EXPECT_STREQ(dataString.data(),"SGVsbG9Xb3JsZA==");
    }
}

TEST(MagicCryptoTest,Base64FileEncoderAndDecoder){
    std::string data;
    Magic::StringView enc = "Base64.enc";
    Magic::StringView dec = "Base64.dec";
    {
        for(auto i = 0;i < 100;i++){
            data.append("XXXX");
        }
        Magic::FileStream fileStream(dec);
        fileStream.open(Magic::FileStream::OpenMode::Write);
        fileStream.write(data);
    }


    {
        std::string dataString;
        auto encStream = std::make_shared<Magic::FileStream>(dec);
        EXPECT_EQ(encStream->open(Magic::FileStream::OpenMode::Read),true);
        auto decStream = std::make_shared<Magic::FileStream>(enc);
        EXPECT_EQ(decStream->open(Magic::FileStream::OpenMode::Write),true);
        Safe<Magic::Base64Encoder> encoder = std::make_shared<Magic::Base64Encoder>(encStream);
        while(!encoder->eof()){
            auto bufferView = encoder->read();
            dataString.append(bufferView.data(),bufferView.size());
            decStream->write(bufferView);
        }
    }

    {
        std::string dataString;
        auto decStream = std::make_shared<Magic::FileStream>(enc);
        EXPECT_EQ(decStream->open(Magic::FileStream::OpenMode::Read),true);
        auto encStream = std::make_shared<Magic::FileStream>(dec);
        EXPECT_EQ(encStream->open(Magic::FileStream::OpenMode::Write),true);
        Safe<Magic::Base64Decoder> decoder = std::make_shared<Magic::Base64Decoder>(decStream);
        while(!decoder->eof()){
            auto bufferView = decoder->read();
            dataString.append(bufferView.data(),bufferView.size());
            encStream->write(bufferView);
        }
        EXPECT_STREQ(dataString.data(),data.data());
    }
}