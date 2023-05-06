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
    class UriErrorCategory:public std::error_category{
    public:
        const char* name() const noexcept override{
            return "Uri Error Code";
        }

        std::string message(int errorCode) const override{
            switch(static_cast<UriErrorCode>(errorCode)){
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
    class Uri:public Noncopyable{
    public:
        explicit Uri(const std::string_view& uri);

        bool hasError() const;

        uint32_t getPort() const;

        std::string_view getUser() const;

        std::string_view getHost() const;

        std::string_view getPath() const;

        std::string_view getQuery() const;

        std::string_view getScheme() const;

        std::string_view getFragment() const;

        void setPort(uint32_t val);

        void setHost(const std::string_view& val);

        void setPath(const std::string_view& val);

        void setUser(const std::string_view& val);

        void setQuery(const std::string_view& val);

        void setScheme(const std::string_view& val);

        void setFragment(const std::string_view& val);

    private:
        bool execute(const std::string_view& uri);

    private:
        bool m_Error;
        /// 端口
        uint32_t m_Port;
        /// 用户信息
        std::string m_User;
        /// host
        std::string m_Host;
        /// 路径
        std::string m_Path;
        /// 查询参数
        std::string m_Query;
        /// scheme
        std::string m_Scheme;
        /// fragment
        std::string m_Fragment;

    };
}
}
}

