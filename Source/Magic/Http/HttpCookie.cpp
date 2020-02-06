#include "Core.h"
#include "Http/HttpCookie.h"

namespace Magic{
namespace Http{

        HttpCookie::HttpCookie(const std::string& key,const std::string& value)
            :m_Secure(false)
            ,m_HttpOnly(false)
            ,m_Key(key)
            ,m_Value(value){
        }
        void HttpCookie::setSecure(bool secure){
            m_Secure = true;
        }
        void HttpCookie::setHttpOnly(bool isHttpOnly){
            m_HttpOnly = isHttpOnly;
        }
        void HttpCookie::setKey(const std::string& key){
            m_Key = key;
        }
        void HttpCookie::setPath(const std::string& path){
            m_Path = path;
        }
        void HttpCookie::setValue(const std::string& value){
            m_Value = value;
        }
        void HttpCookie::setDomain(const std::string& domain){
            m_Domain = domain;
        }
        void HttpCookie::setMaxAge(std::time_t seconds){
            m_MaxAge = seconds;
        }

        std::string& HttpCookie::getKey(){
            return m_Key;
        }
        std::string& HttpCookie::getPath(){
            return m_Path;
        }
        std::string& HttpCookie::getValue(){
            return m_Value;
        }

        std::string HttpCookie::toString(){
            std::string result;
            result.reserve(256);
            result.append(m_Key);
            result.append("=");
            result.append(m_Value);
            if(!m_Path.empty()){
                result.append("; Path=");
                result.append(m_Path);
            }
            if(!m_Domain.empty()){
                result.append("; Domain=");
                result.append(m_Domain);
            }
            if(m_MaxAge != -1){
                result.append("; Expires=");
                result.append(GetTimeGMTString(m_MaxAge));
            }
            if(m_Secure){
                result.append("; Secure");
            }
            if(m_HttpOnly){
                result.append("; HttpOnly");
            }
            return result;
        }

}
}