/*
 ******************************************************************************
 * @Author         : INotfound
 * @File           : Uri.hpp
 * @Date           : 2023-07-03 18:28
 ******************************************************************************
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
        explicit Uri(const StringView& uri);

        bool hasError() const;

        uint32_t getPort() const;

        StringView getUser() const;

        StringView getHost() const;

        StringView getPath() const;

        StringView getQuery() const;

        StringView getScheme() const;

        StringView getFragment() const;

        void setPort(uint32_t val);

        void setHost(const StringView& val);

        void setPath(const StringView& val);

        void setUser(const StringView& val);

        void setQuery(const StringView& val);

        void setScheme(const StringView& val);

        void setFragment(const StringView& val);

    private:
        bool execute(const StringView& uri);

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

