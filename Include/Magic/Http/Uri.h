/*
 * @file: Uri.h
 * @Author: INotFound
 * @Date: 2020-03-15 17:24:20
 * @LastEditTime: 2020-03-15 17:27:48
 */
#pragma once
#include "Core.h"
namespace Magic {
/*
     foo://user@sylar.com:8042/over/there?name=ferret#nose
       \_/   \______________/\_________/ \_________/ \__/
        |           |            |            |        |
     scheme     authority       path        query   fragment
Ragel
*/

/**
 * @brief: Uri类
 */
class Uri {
public:
    Uri();
    bool execute(const std::string& uri);
    uint32_t getPort() const;
    const std::string& getUser() const;
    const std::string& getHost() const;
    const std::string& getPath() const;
    const std::string& getQuery() const;
    const std::string& getScheme() const;
    const std::string& getFragment() const;
    void setPort(uint32_t val);
    void setHost(const std::string& val);
    void setPath(const std::string& val);
    void setUser(const std::string& val);
    void setQuery(const std::string& val);
    void setScheme(const std::string& val);
    void setFragment(const std::string& val);
private:
    bool isDefaultPort() const;
private:
    /// 端口
    uint32_t m_Port;
    /// scheme
    std::string m_Scheme;
    /// 用户信息
    std::string m_User;
    /// host
    std::string m_Host;
    /// 路径
    std::string m_Path;
    /// 查询参数
    std::string m_Query;
    /// fragment
    std::string m_Fragment;

};

}