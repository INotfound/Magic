/*
 * @Author: INotFound
 * @Date: 2020-03-01 15:22:23
 * @LastEditTime: 2021-02-01 22:24:35
 */
#include <cstring>
#include <algorithm>

#include "Magic/Utilty/String.h"
#include "Magic/NetWork/Http/Http.h"

namespace Magic{
namespace NetWork{
namespace Http{
    static const char* g_MethodString[]{
        #define XX(num,name,string) #name,
            HTTP_METHOD_MAP(XX)
        #undef XX
    };

    static const std::map<std::string,std::string,CaseInsensitiveLess> g_HttpContent ={
        #define XX(name,extName,desc) {extName,desc},
            HTTP_CONTENT_TYPE(XX)
        #undef XX
    };

    static const std::map<HttpContentType,std::string> g_HttpContentType ={
        #define XX(name,extName,desc) {HttpContentType::name,desc},
            HTTP_CONTENT_TYPE(XX)
        #undef XX
    };

    bool IsUrlEncode(const std::string& str) {
		return str.find('%') != std::string::npos || str.find('+') != std::string::npos;
	}

    HttpMethod CharsToHttpMethod(const char* str){
        #define XX(num,name,string) \
            if(strncmp(#string,str,strlen(#string)) == 0){ \
                return HttpMethod::name; \
            }
            HTTP_METHOD_MAP(XX);
        #undef XX
        return HttpMethod::INVALID_METHOD;
    }

    HttpMethod StringToHttpMethod(const std::string &str){
        #define XX(num,name,string) \
            if(strcmp(#name,str.c_str()) == 0){ \
                return HttpMethod::name; \
            }
            HTTP_METHOD_MAP(XX);
        #undef XX
        return HttpMethod::INVALID_METHOD;
    }

    const char* HttpMethodToString(const HttpMethod & method){
        auto index = static_cast<uint64_t>(method);
        if(index >= (sizeof(g_MethodString) / sizeof(g_MethodString[0]))){
            return "<unknown>";
        }
        return g_MethodString[index];
    }

    const char* HttpStatusToString(const HttpStatus & status){
        switch(status){
            #define XX(code,name,desc) \
            case HttpStatus::name : \
                return #desc;
            HTTP_STATUS_MAP(XX)
            #undef XX
            default:
                return "<unknown>";
        }
    }

   std::string UrlEncode(const std::string &value) noexcept {
		static auto hex_chars = "0123456789ABCDEF";

		std::string result;
		result.reserve(value.size()); // Minimum size of result

		for (auto &chr : value) {
			if (!((chr >= '0' && chr <= '9') || (chr >= 'A' && chr <= 'Z') || (chr >= 'a' && chr <= 'z') || chr == '-' || chr == '.' || chr == '_' || chr == '~'))
				result += std::string("%") + hex_chars[static_cast<unsigned char>(chr) >> 4] + hex_chars[static_cast<unsigned char>(chr) & 15];
			else
				result += chr;
		}

		return result;
	}

	std::string UrlDecode(const std::string &value) noexcept {
		std::string result;
		result.reserve(value.size() / 3 + (value.size() % 3)); // Minimum size of result

		for (std::size_t i = 0; i < value.size(); ++i) {
			auto &chr = value[i];
			if (chr == '%' && i + 2 < value.size()) {
				auto hex = value.substr(i + 1, 2);
				auto decoded_chr = static_cast<char>(std::strtol(hex.c_str(), nullptr, 16));
				result += decoded_chr;
				i += 2;
			}
			else if (chr == '+')
				result += ' ';
			else
				result += chr;
		}

		return result;
	}

    const char* FileTypeToHttpContentType(const std::string& fileName){
        std::string extName;
        auto pos = fileName.rfind('.');
        if(pos != std::string::npos){
            extName = fileName.substr(pos+1);
            std::transform(extName.begin(),extName.end(),extName.begin(),tolower);
            auto iter = g_HttpContent.find(extName);
            if(iter != g_HttpContent.end()){
                return iter->second.c_str();
            }
        }
        return "application/octet-stream";
    }

    const char* HttpContentTypeToString(const HttpContentType& contentType){
        return g_HttpContentType.at(contentType).c_str();
    }
    
    template<class Map>
    inline void Parse(const std::string& str,Map& map,const std::string& flag){
        uint64_t pos = 0;
        do {
            uint64_t key = 0;
            std::string subString(Split(str, pos, flag));
            if(IsUrlEncode(subString)){
                subString = UrlDecode(subString);
            }
            pos += static_cast<uint64_t>(subString.size() + 1);
            key = subString.find('=');
            if (key == std::string::npos)
                break;
            map.emplace(subString.substr(0, key)
                ,subString.substr(key + 1));
        } while (pos <= str.size());
    } 

    template<class Map>
    inline void ParseCookies(const std::string& str,Map& map,const std::string& flag){
        uint64_t pos = 0;
        do {
            uint64_t key = 0;
            std::string subString(Split(str, pos, flag));
            if(IsUrlEncode(subString)){
                subString = UrlDecode(subString);
            }
            pos += static_cast<uint64_t>(subString.size() + 1);
            key = subString.find('=');
            if (key == std::string::npos)
                break;
            map.emplace(Trim(subString.substr(0, key))
                ,Trim(subString.substr(key + 1)));
        } while (pos <= str.size());
    }

    bool CaseInsensitiveLess::operator()(const std::string& lhs,const std::string& rhs) const{
        return StringCompareNoCase(lhs,rhs) < 0;
    }

    HttpRequest::HttpRequest(bool keepAlive,uint8_t version)
        :m_KeepAlive(keepAlive)
        ,m_Version(version)
        ,m_UrlPath("/"){
    }

    void HttpRequest::setVersion(uint8_t ver){
        m_Version = ver;
    }
    
    void HttpRequest::setMethod(HttpMethod method){
        m_Method = method;
    }

    void HttpRequest::setKeepAlive(bool keepAlive){
        m_KeepAlive = keepAlive;
    }

    void HttpRequest::setBody(const std::string& body){
        auto contentType = m_Headers.find("Content-Type");
        if(contentType != m_Headers.end() && StringCompareNoCase(contentType->second,"application/x-www-form-urlencoded") == 0){
            Parse(body,m_Params,"&");
        }
        m_Body = body;
    }

    void HttpRequest::setQuery(const std::string& query){
        Parse(query,m_Params,"&");
        m_Query = query;
    }
    
    void HttpRequest::setPath(const std::string& urlPath){
        m_UrlPath = urlPath;
    }

    void HttpRequest::setFragment(const std::string& fragment){
        m_Fragment = fragment;
    }

    void HttpRequest::setParam(const std::string& key,const std::string& value){
        m_Params.emplace(key, value);
    }
    
    void HttpRequest::setHeader(const std::string& key,const std::string& value){
        m_Headers.emplace(key, value);
    }

    void HttpRequest::setCookie(const std::string& key,const std::string& value){
        m_Cookies.emplace(key, value);
    }

    HttpRequest::KeyValue& HttpRequest::atParams() {
        return m_Params;
    }

    HttpRequest::KeyValue& HttpRequest::atHeaders() {
        return m_Headers;
    }

    bool HttpRequest::getKeepAlive() const{
        return m_KeepAlive;
    }
    
    uint8_t HttpRequest::getVersion() const{
        return m_Version;
    }

    HttpMethod HttpRequest::getMethod() const{
        return m_Method;
    }

    const std::string& HttpRequest::getPath() const{
        return m_UrlPath;
    }

    const std::string& HttpRequest::getBody() const{
        return m_Body;
    }

    const std::string& HttpRequest::getQuery() const{
        return m_Query;
    }

    const std::string& HttpRequest::getCookie(const std::string& key) {
        if(!m_Cookies.empty()){
            auto value = m_Cookies.find(key);
            if(value == m_Cookies.end()){
                return g_EmptyString;
            }
            return value->second;
        }
        auto iter = m_Headers.find("Cookie");
        if(iter == m_Headers.end())
            return g_EmptyString;
        ParseCookies(iter->second,m_Cookies,";");
        auto value = m_Cookies.find(key);
        if(value == m_Cookies.end()){
            return g_EmptyString;
        }
        return value->second;
    }

    const std::string& HttpRequest::getParam(const std::string& key)const{
        auto iter = m_Params.find(key);
        if(iter == m_Params.end()){
            return g_EmptyString;
        }
        return iter->second;
    }

    const std::string& HttpRequest::getHeader(const std::string& key)const{
        auto iter = m_Headers.find(key);
        if(iter == m_Headers.end()){
            return g_EmptyString;
        }
        return iter->second;
    }

    void HttpRequest::delParam(const std::string& key){
        m_Params.erase(key);
    }

    void HttpRequest::delHeader(const std::string& key){
        m_Headers.erase(key);
    }
    
    std::ostream& HttpRequest::toStream(std::ostream& os){
        os  << HttpMethodToString(m_Method) << ' '
            << m_UrlPath
            << (m_Query.empty() ? "" : "?")
            << m_Query
            << (m_Fragment.empty() ? "" : "#")
            << m_Fragment << ' '
            << "HTTP/"
            << static_cast<uint32_t>(m_Version >> 4)
            << '.'
            << static_cast<uint32_t>(m_Version & 0x0F)
            << "\r\n"
            << "Connection: " << (m_KeepAlive ? "keep-alive" : "close") << "\r\n";
        
        for(auto& v : m_Headers){
            if(StringCompareNoCase(v.first,"Connection") == 0){
                continue;
            }
            os << v.first << ": " << v.second << "\r\n";
        }

        if(!m_Body.empty()){
            os  << "Content-Length: " << m_Body.size() << "\r\n\r\n"
                << m_Body;
        }else{
            os  << "\r\n";
        }
        return os;
    }

    HttpResponse::HttpResponse(bool keepAlive,uint8_t version)
        :m_KeepAlive(keepAlive)
        ,m_Version(version)
        ,m_Status(HttpStatus::OK)
        ,m_ContentType(HttpContentType::TEXT_HTML){
    }

    void HttpResponse::setVersion(uint8_t ver){
        m_Version = ver;
    }

    void HttpResponse::setStatus(HttpStatus status){
        m_Status = status;
    }

    void HttpResponse::setKeepAlive(bool keepAlive){
        m_KeepAlive = keepAlive;
    }

    void HttpResponse::setBody(const std::string& body){
        m_Body = body;
    }

    void HttpResponse::setReason(const std::string& reason){
        m_Reason = reason;
    }

    void HttpResponse::setContentType(const std::string& contentType){
        m_ContentTypeString = contentType;
    }

    void HttpResponse::setContentType(const HttpContentType& contentType){
        m_ContentTypeString.clear();
        m_ContentType = contentType;
    }

    void HttpResponse::setHeader(const std::string& key,const std::string& value){
        m_Headers.emplace(key,value);
    }
    
    void HttpResponse::setCookie(const std::string& key, const std::string& val,time_t expired
            ,const std::string& path,const std::string& domain,bool httpOnly,bool secure){
        std::string result;
        result.reserve(256);
        result.append(key);
        result.append("=");
        result.append(val);
        if(!path.empty()){
            result.append("; Path=");
            result.append(path);
        }
        if(!domain.empty()){
            result.append("; Domain=");
            result.append(domain);
        }
        if(expired > 0){
            result.append("; Expires=");
            result.append(GetTimeGMTString(expired));
        }
        if(httpOnly){
            result.append("; HttpOnly");
        }
        if(secure){
            result.append("; Secure");
        }
        m_Cookies.push_back(result);
    }

    bool HttpResponse::getKeepAlive() const{
        return m_KeepAlive;
    }

    uint8_t HttpResponse::getVersion() const{
        return m_Version;
    }

    HttpStatus HttpResponse::getStatus() const{
        return m_Status;
    }

    const std::string& HttpResponse::getBody() const{
        return m_Body;
    }
    
    const std::string& HttpResponse::getReason() const{
        return m_Reason;
    }

    const std::string& HttpResponse::getHeader(const std::string& key){
        auto iter = m_Headers.find(key);
        if(iter == m_Headers.end()){
            return g_EmptyString;
        }
        return iter->second;
    }

    void HttpResponse::delHeader(const std::string& key){
        m_Headers.erase(key);
    }

    std::ostream& HttpResponse::toStream(std::ostream& os){
        os  << "HTTP/"
            << static_cast<uint32_t>(m_Version >> 4)
            << '.'
            << static_cast<uint32_t>(m_Version & 0x0F)
            << ' '
            << static_cast<uint32_t>(m_Status)
            << ' '
            << (m_Reason.empty() ? HttpStatusToString(m_Status) : m_Reason)
            << "\r\n"
            << "Connection: " << (m_KeepAlive ? "keep-alive" : "close") << "\r\n";

        if(m_ContentTypeString.empty()){
            os << "Content-Type: " << HttpContentTypeToString(m_ContentType) << "\r\n";
        }else{
            os << "Content-Type: " << m_ContentTypeString << "\r\n";
        }

        for(auto& v : m_Headers){
            if(StringCompareNoCase(v.first,"Connection") == 0 || StringCompareNoCase(v.first,"Content-Type") == 0){
                continue;
            }
            os << v.first << ": " << v.second << "\r\n";
        }
        
        for(auto& v : m_Cookies){
            os << "Set-Cookie: " << v << "\r\n";
        }

        if(!m_Body.empty()){
            os  << "Content-Length: " << m_Body.size() << "\r\n\r\n"
                << m_Body;
        }else{
            os << "\r\n";
        }
        return os;
    }
    
    std::ostream& operator<<(std::ostream& os, const Safe<HttpRequest>& request){
        return request->toStream(os);
    }

    std::ostream& operator<<(std::ostream& os, const Safe<HttpResponse>& response){
        return response->toStream(os);
    }
}
}
}
