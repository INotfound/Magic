/*
 * @File: MultiPart.h
 * @Author: INotFound
 * @Date: 2020-03-15 17:10:22
 * @LastEditTime: 2020-04-06 19:50:55
 */
#pragma once
#include "Http.h"
#include "HttpFile.h"

#include "../Core.h"
namespace Magic{
namespace Http{
    /**
     * @brief: MultiPart类
     */
    class MultiPart
    {
    public:
        /**
         * @brief: 析构函数
         */
        ~MultiPart();
        /**
         * @brief: 构造函数
         */
        MultiPart();
        /**
         * @brief: 解析函数
         * @param request 需要解析请求的对象
         * @return: 返回True则成功,返回False则失败
         */
        bool parse(const Safe<HttpRequest>& request);
        /**
         * @brief: 获取多个文件函数
         * @return: 返回文件集合
         */
        const std::vector<Safe<HttpFile>>& getFiles() const;
        /**
         * @brief: 获取参数集函数
         * @return: 返回参数集
         */
        const std::map<std::string,std::string>& getParams() const;
    private:
        /**
         * @brief: 解析实体函数
         * @param begin 数据开始的位置
         * @param end 数据结束位置
         * @return: 返回True则成功,返回False则失败
         */
        bool parseEntity(const char* begin,const char* end);
    private:
        std::vector<Safe<HttpFile>> m_Files;
        std::map<std::string,std::string> m_Params;
    };
}
}