#include "gtest/gtest.h"
#include <cctype>
#include <cstdio>
#include "Magic/Core/Stream.hpp"
#include "Magic/Core/StringView.hpp"

TEST(MagicStreamTest,DataStream){
    Magic::DataStream dataStream("Data");
    EXPECT_EQ(dataStream.size(),4);
    dataStream.write("Data");
    EXPECT_EQ(dataStream.size(),8);
    std::string text;
    while(!dataStream.eof()){
        Magic::StringView buffer = dataStream.read();
        text.append(buffer.data(),buffer.size());
    }
    EXPECT_EQ(text.size(),8);
}

TEST(MagicStreamTest,FileStream){
    Magic::FileStream fileStreamWrite("Data.txt");
    EXPECT_EQ(fileStreamWrite.open(Magic::FileStream::OpenMode::Write),true);
    for(auto i = 0;i < 10000;i++){
        fileStreamWrite.write("XXXX");
    }
    auto fileSize = fileStreamWrite.size();
    Magic::FileStream fileStreamRead("Data.txt");
    EXPECT_EQ(fileStreamRead.open(Magic::FileStream::OpenMode::Read),true);
    std::string text;
    while(!fileStreamRead.eof()){
        Magic::StringView buffer = fileStreamRead.read();
        text.append(buffer.data(),buffer.size());
    }
    EXPECT_EQ(text.size(),fileSize);
}

TEST(MagicStreamTest,ReadWriteFileStream){
    Magic::FileStream fileStreamReadWrite("Data.dt");
    EXPECT_EQ(fileStreamReadWrite.open(Magic::FileStream::OpenMode::ReadWrite),true);
    fileStreamReadWrite.write("Data");
    fileStreamReadWrite.seek(0);
    auto buffer = fileStreamReadWrite.read();
    EXPECT_TRUE(buffer.size() > 0);
}