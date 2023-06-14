#include <fstream>
#include "gtest/gtest.h"
#include "Magic/Utilty/Crypto.hpp"

TEST(MagicCryptoTest,Base64Encode){
    EXPECT_STREQ(Magic::Base64Encode("HelloWorld").c_str(),"SGVsbG9Xb3JsZA==");
}

TEST(MagicCryptoTest,Base64Decode){
    EXPECT_STREQ(Magic::Base64Decode("SGVsbG9Xb3JsZA==").c_str(),"HelloWorld");;
}

TEST(MagicCryptoTest,StringToHexMD5){
    EXPECT_STREQ(Magic::StringToHexMD5("Hello World").c_str(),"b10a8db164e0754105b7a99be72e3fe5");
}

TEST(MagicCryptoTest,StringToHexSHA1){
    EXPECT_STREQ(Magic::StringToHexSHA1("Hello World").c_str(),"0a4d55a8d778e5022fab701977c5d840bbc486d0");
}

TEST(MagicCryptoTest,FileToHexMD5String){
    std::fstream fileStream;
    fileStream.open("CryptoTestMd5.txt",std::ios::out|std::ios::trunc);
    if(fileStream.is_open()){
        fileStream << "Hello World";
        fileStream.flush();
    }
    fileStream.close();
    EXPECT_STREQ(Magic::FileToHexMD5String("CryptoTestMd5.txt").c_str(),"b10a8db164e0754105b7a99be72e3fe5");
}