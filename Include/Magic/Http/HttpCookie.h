#pragma once
#include <ctime>

namespace Magic{
namespace Http{
    class HttpCookie{
    public:
        HttpCookie(const std::string& key,const std::string& value);
        void setSecure(bool secure);
        void setHttpOnly(bool isHttpOnly);
        void setKey(const std::string& key);
        void setPath(const std::string& path);
        void setValue(const std::string& value);
        void setDomain(const std::string& domain);
        void setMaxAge(std::time_t seconds);

        std::string& getKey();
        std::string& getPath();
        std::string& getValue();

        std::string toString();
    private:
        bool m_Secure;
        bool m_HttpOnly;
        std::string m_Key;
        std::string m_Path;
        std::string m_Value;
        std::string m_Domain;
        std::time_t m_MaxAge;
    };
}
}