/*
 * @Author: INotFound
 * @Date: 2020-12-17 15:31:24
 * @LastEditTime: 2021-02-01 23:09:16
 */
#pragma once
#include "Base.h"

// 注入示例
namespace Base{
    class TestLog{
    public:
        TestLog(){
            MAGIC_DEBUG() << "Testing";
        }
        void printf(const std::string& msg){
            MAGIC_DEBUG() <<  msg;
        }
    };
}
