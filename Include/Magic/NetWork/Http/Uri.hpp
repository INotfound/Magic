/*
 * @File: Uri.hpp
 * @Author: INotFound
 * @Date: 2020-03-15 17:24:20
 * @LastEditTime: 2021-01-18 05:54:03
 */
#pragma once

#include "Magic/Core/Core.hpp"

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
        explicit Uri(const Magic::StringView& uri);

        bool hasError() const;

        uint32_t getPort() const;

        Magic::StringView getUser() const;

        Magic::StringView getHost() const;

        Magic::StringView getPath() const;

        Magic::StringView getQuery() const;

        Magic::StringView getScheme() const;

        Magic::StringView getFragment() const;

        void setPort(uint32_t val);

        void setHost(const Magic::StringView& val);

        void setPath(const Magic::StringView& val);

        void setUser(const Magic::StringView& val);

        void setQuery(const Magic::StringView& val);

        void setScheme(const Magic::StringView& val);

        void setFragment(const Magic::StringView& val);

    private:
        bool execute(const Magic::StringView& uri);

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

