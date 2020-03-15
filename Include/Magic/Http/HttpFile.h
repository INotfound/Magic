/*
 * @file: HttpFile.h
 * @Author: INotFound
 * @Date: 2020-03-15 16:53:09
 * @LastEditTime: 2020-03-15 17:10:09
 */
#pragma once
#include <fstream>
namespace Magic{
namespace Http{
    /**
     * @brief: Http文件类
     */
    class HttpFile{
    public:
        /**
         * @brief: 析构函数
         */
        ~HttpFile();
        /**
         * @brief: 构造函数
         * @param name 文件名称
         * @param content 文件内容
         */
        HttpFile(const std::string& name,const std::string& content);
        /**
         * @brief: 保存文件函数
         * @param path 保存路径
         * @return: 返回True则成功,返回False则失败
         */        
        bool save(const std::string& path);
        /**
         * @brief: 获取文件名称函数
         * @return: 返回文件名称
         */
        const std::string getName() const;
    private:
        std::string m_Name;
        std::string m_Content;
        std::ofstream m_FileStream;
    };

}
}