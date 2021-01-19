/*
 * @Author: INotFound
 * @Date: 2020-12-17 15:31:24
 * @LastEditTime: 2021-01-12 06:00:15
 */
#pragma once
#include <Magic.h>

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
