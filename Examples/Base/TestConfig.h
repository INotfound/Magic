/*
 * @Author: INotFound
 * @Date: 2020-12-17 15:31:24
 * @LastEditTime: 2021-01-13 07:44:12
 */
#pragma once
#include "TestLog.h"
#include "Examples/Base/Base.h"
// 注入示例
namespace Base{
    class TestConfig{
    public:
        TestConfig(const Safe<Magic::Config>& configuration){
            uint32_t num = configuration->at("Number",0);
            MAGIC_DEBUG() << "get Config Value[Number]: " << num;
        }
        void call(const Safe<Base::TestLog>& log){
            log->printf("testttttttt");
        }
    };
}
