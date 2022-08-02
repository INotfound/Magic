/*
 * @Author: INotFound
 * @Date: 2020-12-17 15:31:24
 * @LastEditTime: 2021-02-01 23:09:40
 */
#pragma once
#include "Examples/Base/Base.h"

// 注入示例
namespace Base{
    class Test{
    public:
        Test(){
            std::printf("Testing\n");
        }
        void printf(const std::string& msg){
            std::printf("%s\n",msg.c_str());
        }
    };
}
