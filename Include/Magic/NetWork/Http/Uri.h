/*
 * @File: Uri.h
 * @Author: INotFound
 * @Date: 2020-03-15 17:24:20
 * @LastEditTime: 2021-01-18 05:54:03
 */
#pragma once
#include "Magic/Core/Core.h"

namespace Magic{
namespace NetWork{
namespace Http{
    /**
     * @brief UriErrorCode
     */
    enum class UriErrorCode{
        NoError = 0,
        ParseError
    };
    /**
     * @brief UriErrorCategory
     */
    class UriErrorCategory :public std::error_category{
    public:
        const char* name() const noexcept override{
            return "Uri Error Code";
        }
        std::string message(int errorCode) const override{
            switch (static_cast<UriErrorCode>(errorCode)) {
                case UriErrorCode::NoError:
                    return "No Error";
                    break;
                case UriErrorCode::ParseError:
                    return "Uri Parse Error";
                    break;
            }
            return std::string();
        }
    };
    /**
     * @brief Uri类
     */
    class Uri {
    public:
        explicit Uri(const std::string& uri);
        bool hasError() const;
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
        bool execute(const std::string& uri);
    private:
        bool m_Error;
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
}
}

