#include "gtest/gtest.h"
#include "Magic/Utilty/Compress.h"

TEST(MagicCompressTest,BrotliEncoderAndBrotliDecoder){
    for(auto i = 0;i < 1000;i++){
        std::string compressData;
        std::string unCompressData;
        std::string data = "AASDFGHJKSDFGHJKDFGHJKCVBNRGHJK";
        Magic::BrotliEncoder(data,compressData);
        Magic::BrotliDecoder(compressData,unCompressData);
        EXPECT_STREQ(data.c_str(),unCompressData.c_str());
    }
}

#ifdef ZLIB
TEST(MagicCompressTest,GZipEncoderAndBrotliDecoder){
    for(auto i = 0;i < 1000;i++){
        std::string compressData;
        std::string unCompressData;
        std::string data = "AASDFGHJKSDFGHJKDFGHJKCVBNRGHJK";
        Magic::GZipEncoder(data,compressData);
        Magic::GZipDecoder(compressData,unCompressData);
        EXPECT_STREQ(data.c_str(),unCompressData.c_str());
    }
}
#endif