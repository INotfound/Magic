/*
 * @file: Test.cpp
 * @Author: INotFound
 * @Date: 2020-03-12 01:59:04
 * @LastEditTime: 2020-03-19 23:25:06
 */
#include <iostream>
#include "Magic.h"

std::vector<int> Byte;
//1byte: btye1
//2byte: byte1-128 + byte*128
//3byte: byte1-128 + (byte2-128 + byte3*128)*128
//4byte: byte1-128 + (byte2-128 + (byte3-128 + byte4*128)*128)*128
void MQTTEncodeLastLength(int length){
    for (size_t i = 0; i < 4; i++){
        int encodedByte = length % 128;
        length = length / 128;
        if(length > 0){
            encodedByte = encodedByte | 128;
            Byte.push_back(encodedByte);
            continue;
        }
        if(encodedByte != 0)
            Byte.push_back(encodedByte);
    }
}
void MQTTDencodeLastLength(std::vector<int>& Byte){
    int len = Byte.size();
    if(len == 1){
        //....
    }else if(len == 2){
        auto b1 = Byte.at(0);
        auto b2 = Byte.at(1);
        auto length = b1-128 + b2*128;
        MAGIC_DEBUG() << length;
    }else if(len == 3){
        auto b1 = Byte.at(0);
        auto b2 = Byte.at(1);
        auto b3 = Byte.at(2);
        auto length = b1-128 + (b2-128 + b3*128)*128;
        MAGIC_DEBUG() << length;
    }else if(len == 4){
        auto b1 = Byte.at(0);
        auto b2 = Byte.at(1);
        auto b3 = Byte.at(2);
        auto b4 = Byte.at(3);
        auto length = b1-128 + (b2-128 + (b3-128 + b4*128)*128)*128;
        MAGIC_DEBUG() << length;
    }
}

int main() {
    Magic::Init("Build_Test");
    MQTTEncodeLastLength(2097160);
    MQTTDencodeLastLength(Byte);
    getchar();
    return 0;
}


