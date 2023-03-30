/*
 * @Author: INotFound
 * @Date: 2020-03-01 15:22:23
 * @LastEditTime: 2021-02-01 22:24:35
 */
#include <cstring>
#include <algorithm>

#include "Magic/Utilty/Compress.h"
#include "Magic/NetWork/Http/Http.h"

namespace Magic{
namespace NetWork{
namespace Http{
    static const char* g_MethodString[]{
        #define XX(num,name,string) #name,
            HTTP_METHOD_MAP(XX)
        #undef XX
    };

    static const std::map<std::string,HttpContentType,CaseInsensitiveLess> g_HttpContent = {
        #define XX(name,extName,desc){extName,HttpContentType::name},
            HTTP_CONTENT_TYPE(XX)
        #undef XX
    };

    static const std::map<HttpContentType,std::string> g_HttpContentType = {
        #define XX(name,extName,desc){HttpContentType::name,desc},
            HTTP_CONTENT_TYPE(XX)
        #undef XX
    };

    bool IsUrlEncode(const std::string& str){
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

    HttpMethod StringToHttpMethod(const std::string& str){
        #define XX(num,name,string) \
            if(strcmp(#name,str.data()) == 0){ \
                return HttpMethod::name; \
            }
            HTTP_METHOD_MAP(XX);
        #undef XX
        return HttpMethod::INVALID_METHOD;
    }

    const char* HttpMethodToString(const HttpMethod& method){
        auto index = static_cast<uint64_t>(method);
        if(index >= (sizeof(g_MethodString) / sizeof(g_MethodString[0]))){
            return "<unknown>";
        }
        return g_MethodString[index];
    }

    const char* HttpStatusToString(const HttpStatus& status){
        switch(status){
        #define XX(code,name,desc)  \
            case HttpStatus::name : \
                return #desc;
            HTTP_STATUS_MAP(XX)
        #undef XX
            default:
                return "<unknown>";
        }
    }

    std::string UrlEncode(const std::string& value) noexcept{
        static auto hex_chars = "0123456789ABCDEF";

        std::string result;
        result.reserve(value.size()); // Minimum size of result

		for (auto &chr : value){
			if (!((chr >= '0' && chr <= '9')
			    || (chr >= 'A' && chr <= 'Z')
			    || (chr >= 'a' && chr <= 'z')
			    || chr == '=' || chr == '-'
			    || chr == '&' || chr == '_'
			    || chr == '~' || chr == '.'))
				result += std::string("%") + hex_chars[static_cast<unsigned char>(chr) >> 4] + hex_chars[static_cast<unsigned char>(chr) & 15];
			else
				result += chr;
		}
		return result;
	}

    std::string UrlDecode(const std::string& value) noexcept{
        std::string result;
        result.reserve(value.size() / 3 + (value.size() % 3)); // Minimum size of result

        for(std::size_t i = 0;i < value.size();++i){
            auto& chr = value[i];
            if(chr == '%' && i + 2 < value.size()){
                auto hex = value.substr(i + 1,2);
                auto decoded_chr = static_cast<char>(std::strtol(hex.data(),nullptr,16));
                result += decoded_chr;
                i += 2;
            }else if(chr == '+')
                result += ' ';
            else
                result += chr;
        }

        return result;
    }

    HttpContentType FileTypeToHttpContentType(const std::string& fileName){
        std::string extName;
        auto pos = fileName.rfind('.');
        if(pos != std::string::npos){
            extName = fileName.substr(pos + 1);
            std::transform(extName.begin(),extName.end(),extName.begin(),::toupper);
            auto iter = g_HttpContent.find(extName);
            if(iter != g_HttpContent.end()){
                return iter->second;
            }
        }
        return HttpContentType::APPLICATION_OCTET_STREAM;
    }

    const char* HttpContentTypeToString(const HttpContentType& contentType){
        return g_HttpContentType.at(contentType).data();
    }

    template<typename Map>
    inline void Parse(const std::string& str,Map& map,const std::string& flag){
        uint64_t pos = 0;
        do{
            uint64_t key = 0;
            std::string subString(SubString(str,pos,flag));
            if(IsUrlEncode(subString)){
                subString = UrlDecode(subString);
            }
            pos += static_cast<uint64_t>(subString.size() + 1);
            key = subString.find('=');
            if(key == std::string::npos)
                break;
            map.emplace(subString.substr(0,key),subString.substr(key + 1));
        }while(pos <= str.size());
    }

    template<typename Map>
    inline void ParseCookies(const std::string& str,Map& map,const std::string& flag){
        uint64_t pos = 0;
        do{
            uint64_t key = 0;
            std::string subString(SubString(str,pos,flag));
            if(IsUrlEncode(subString)){
                subString = UrlDecode(subString);
            }
            pos += static_cast<uint64_t>(subString.size() + 1);
            key = subString.find('=');
            if(key == std::string::npos)
                break;
            map.emplace(Trim(subString.substr(0,key)),Trim(subString.substr(key + 1)));
        }while(pos <= str.size());
    }

    inline std::string GenerateHtml(const std::string& status,const std::string& title){
        std::stringstream html;
        html << "<!DOCTYPE html><html lang=\"en\"><head><title>"
             << status
             << "</title></head><body><center><h1>"
             << title
             << "</h1></center><hr><center>Magic/2.0.0</center></body></html>";
        return html.str();
    }

    HttpRequest::HttpRequest(bool keepAlive,uint8_t version)
        :m_KeepAlive(keepAlive)
        ,m_Version(version)
        ,m_Method(HttpMethod::GET)
        ,m_UrlPath("/")
        ,m_ContentLength(0){
    }

    HttpRequest::KeyValue& HttpRequest::atParams(){
        return m_Params;
    }

    HttpRequest::KeyValue& HttpRequest::atHeaders(){
        return m_Headers;
    }

    bool HttpRequest::isRange() const{
        return m_Headers.find("Range") != m_Headers.end();
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

    uint64_t HttpRequest::getRangeStop() const{
        auto iter = m_Headers.find("Range");
        if(iter != m_Headers.end()){
            std::string value = iter->second;
            auto splitPos = value.find('-');
            try{
                return StringAs<uint64_t>(value.substr(splitPos + 1,value.length() - splitPos),10);
            }catch(...){
                return 0;
            }
        }
        return 0;
    }

    uint64_t HttpRequest::getRangeStart() const{
        auto iter = m_Headers.find("Range");
        if(iter != m_Headers.end()){
            std::string value = iter->second;
            auto splitPos = value.find('-');
            try{
                return StringAs<uint64_t>(value.substr(6,splitPos - 6),10);
            }catch(...){
                return 0;
            }
        }
        return 0;
    }

    uint64_t HttpRequest::getContentLength() const{
        return m_ContentLength;
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

    const std::string& HttpRequest::getCookie(const std::string& key){
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

    const std::string& HttpRequest::getParam(const std::string& key) const{
        auto iter = m_Params.find(key);
        if(iter == m_Params.end()){
            return g_EmptyString;
        }
        return iter->second;
    }

    const std::string& HttpRequest::getHeader(const std::string& key) const{
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
            << (m_Query.empty() ? "" : "?" + UrlEncode(m_Query))
            << (m_Fragment.empty() ? "" : "#" + m_Fragment)
            << ' '
            << "HTTP/"
            << static_cast<uint32_t>(m_Version >> 4)
            << '.'
            << static_cast<uint32_t>(m_Version & 0x0F)
            << "\r\n";

        if(m_Headers.find("Connection") == m_Headers.end()){
            os << "Connection: " << (m_KeepAlive ? "keep-alive" : "close") << "\r\n";
        }

        for(auto& v : m_Headers){
            os << v.first << ": " << v.second << "\r\n";
        }

        if(!m_Body.empty()){
            os  << "Content-Length: " << m_Body.size() << "\r\n\r\n"
                << m_Body;
        }else{
            if(m_ContentLength != 0){
                os << "Content-Length: " << m_ContentLength << "\r\n";
            }
            os << "\r\n";
        }
        return os;
    }

    ObjectWrapper<HttpRequest> HttpRequest::setVersion(uint8_t ver){
        m_Version = ver;
        return ObjectWrapper<HttpRequest>(this);
    }

    ObjectWrapper<HttpRequest> HttpRequest::setMethod(HttpMethod method){
        m_Method = method;
        return ObjectWrapper<HttpRequest>(this);
    }

    ObjectWrapper<HttpRequest> HttpRequest::setKeepAlive(bool keepAlive){
        m_KeepAlive = keepAlive;
        return ObjectWrapper<HttpRequest>(this);
    }

    ObjectWrapper<HttpRequest> HttpRequest::setBody(const std::string& body){
        auto contentType = m_Headers.find("Content-Type");
        if(contentType != m_Headers.end() && StringCompareNoCase(contentType->second,"application/x-www-form-urlencoded") == 0){
            Parse(body,m_Params,"&");
        }
        m_Body = body;
        return ObjectWrapper<HttpRequest>(this);
    }

    ObjectWrapper<HttpRequest> HttpRequest::setContentLength(uint64_t length){
        m_ContentLength = length;
        return ObjectWrapper<HttpRequest>(this);
    }

    ObjectWrapper<HttpRequest> HttpRequest::setQuery(const std::string& query){
        Parse(query,m_Params,"&");
        m_Query = query;
        return ObjectWrapper<HttpRequest>(this);
    }

    ObjectWrapper<HttpRequest> HttpRequest::setPath(const std::string& urlPath){
        m_UrlPath = urlPath;
        return ObjectWrapper<HttpRequest>(this);
    }

    ObjectWrapper<HttpRequest> HttpRequest::setRange(uint64_t start,uint64_t stop){
        std::string value("bytes=");
        value.append(AsString<uint64_t>(start));
        value.append("-");
        value += (stop == 0) ? "" : AsString<uint64_t>(stop);
        m_Headers.emplace("Range",value);
        return ObjectWrapper<HttpRequest>(this);
    }

    ObjectWrapper<HttpRequest> HttpRequest::setFragment(const std::string& fragment){
        m_Fragment = fragment;
        return ObjectWrapper<HttpRequest>(this);
    }

    ObjectWrapper<HttpRequest> HttpRequest::setParam(const std::string& key,const std::string& value){
        m_Params.emplace(key,value);
        return ObjectWrapper<HttpRequest>(this);
    }

    ObjectWrapper<HttpRequest> HttpRequest::setHeader(const std::string& key,const std::string& value){
        m_Headers.emplace(key,value);
        return ObjectWrapper<HttpRequest>(this);
    }

    ObjectWrapper<HttpRequest> HttpRequest::setCookie(const std::string& key,const std::string& value){
        m_Cookies.emplace(key,value);
        return ObjectWrapper<HttpRequest>(this);
    }

    HttpResponse::HttpResponse(bool keepAlive,uint8_t version)
        :m_KeepAlive(keepAlive)
        ,m_Version(version)
        ,m_Status(HttpStatus::OK)
        ,m_ContentLength(0)
        ,m_ContentType(HttpContentType::TEXT_PLAIN){
    }

    bool HttpResponse::isRange() const{
        return m_Headers.find("Content-Range") != m_Headers.end();
    }

    bool HttpResponse::hasResource() const{
        return !m_Resource.empty();
    }

    HttpResponse::KeyValue& HttpResponse::getHeaders(){
        return m_Headers;
    }

    bool HttpResponse::getKeepAlive() const{
        return m_KeepAlive;
    }

    uint8_t HttpResponse::getVersion() const{
        return m_Version;
    }

    uint64_t HttpResponse::getRangeStop() const{
        auto iter = m_Headers.find("Content-Range");
        if(iter != m_Headers.end()){
            std::string value = iter->second;
            auto firstSplitPos = value.find('-');
            auto secondSplitPos = value.find('/');
            if(firstSplitPos != std::string::npos && secondSplitPos != std::string::npos){
                try{
                    return StringAs<uint64_t>(value.substr(firstSplitPos + 1,secondSplitPos - firstSplitPos),10);
                }catch(...){
                    return 0;
                }
            }
        }
        return 0;
    }

    HttpStatus HttpResponse::getStatus() const{
        return m_Status;
    }

    uint64_t HttpResponse::getRangeStart() const{
        auto iter = m_Headers.find("Content-Range");
        if(iter != m_Headers.end()){
            std::string value = iter->second;
            auto secondSplitPos = value.find('-');
            std::string::size_type firstSplitPos = 5;
            if(secondSplitPos != std::string::npos){
                try{
                    return StringAs<uint64_t>(value.substr(firstSplitPos + 1,secondSplitPos - firstSplitPos),10);
                }catch(...){
                    return 0;
                }
            }
        }
        return 0;
    }

    uint64_t HttpResponse::getContentLength() const{
        return m_ContentLength;
    }

    const std::string& HttpResponse::getBody() const{
        return m_Body;
    }

    uint64_t HttpResponse::getRangeTotalLength() const{
        auto iter = m_Headers.find("Content-Range");
        if(iter != m_Headers.end()){
            std::string value = iter->second;
            auto firstSplitPos = value.find('/');
            auto secondSplitPos = value.length();
            if(firstSplitPos != std::string::npos && secondSplitPos != std::string::npos){
                try{
                    return StringAs<uint64_t>(value.substr(firstSplitPos + 1,secondSplitPos - firstSplitPos),10);
                }catch(...){
                    return 0;
                }
            }
        }
        return 0;
    }

    const std::string& HttpResponse::getReason() const{
        return m_Reason;
    }

    const std::string& HttpResponse::getResource() const{
        return m_Resource;
    }

    HttpContentType HttpResponse::getContentType() const{
        return m_ContentType;
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
            << "\r\n";

        if(m_Headers.find("Connection") == m_Headers.end()){
            os << "Connection: " << (m_KeepAlive ? "keep-alive" : "close") << "\r\n";
        }

        bool hasBody = !m_Body.empty();

        if(!hasBody && m_Status > HttpStatus::OK){
            hasBody = true;
            m_Body = GenerateHtml(HttpStatusToString(m_Status),HttpStatusToString(m_Status));
        }

        auto contentEncodingIter = m_Headers.find("Content-Encoding");

        if(contentEncodingIter != m_Headers.end()){
            if(hasBody && contentEncodingIter->second.find("br") != std::string::npos){
                std::string compressData;
                contentEncodingIter->second = "br";
                Magic::BrotliEncoder(m_Body,compressData);
                m_Body.swap(compressData);
            }
        #ifdef ZLIB
            else if(hasBody && contentEncodingIter->second.find("gzip") != std::string::npos){
                std::string compressData;
                contentEncodingIter->second = "gzip";
                Magic::GZipEncoder(m_Body,compressData);
                m_Body.swap(compressData);
            }
        #endif
            else{
                m_Headers.erase(contentEncodingIter);
            }

        }

        /// Headers
        for(auto& v : m_Headers){
            os << v.first << ": " << v.second << "\r\n";
        }

        for(auto& v : m_Cookies){
            os << "Set-Cookie: " << v << "\r\n";
        }

        if(hasBody){
            os  << "Content-Length: " << m_Body.size() << "\r\n\r\n"
                << m_Body;
        }else{
            os << "Content-Length: " << m_ContentLength << "\r\n\r\n";
        }
        return os;
    }

    ObjectWrapper<HttpResponse> HttpResponse::setVersion(uint8_t ver){
        m_Version = ver;
        return ObjectWrapper<HttpResponse>(this);
    }

    ObjectWrapper<HttpResponse> HttpResponse::setStatus(HttpStatus status){
        m_Status = status;
        return ObjectWrapper<HttpResponse>(this);
    }

    ObjectWrapper<HttpResponse> HttpResponse::setKeepAlive(bool keepAlive){
        m_KeepAlive = keepAlive;
        return ObjectWrapper<HttpResponse>(this);
    }

    ObjectWrapper<HttpResponse> HttpResponse::setBody(const std::string& body){
        m_Body = body;
        return ObjectWrapper<HttpResponse>(this);
    }

    ObjectWrapper<HttpResponse> HttpResponse::setContentLength(uint64_t length){
        m_ContentLength = length;
        return ObjectWrapper<HttpResponse>(this);
    }

    ObjectWrapper<HttpResponse> HttpResponse::setReason(const std::string& reason){
        m_Reason = reason;
        return ObjectWrapper<HttpResponse>(this);
    }

    ObjectWrapper<HttpResponse> HttpResponse::setResource(const std::string& filePath){
        m_Resource = filePath;
        return ObjectWrapper<HttpResponse>(this);
    }

    ObjectWrapper<HttpResponse> HttpResponse::setContentType(const std::string& contentType){
        m_Headers["Content-Type"] = contentType;
        return ObjectWrapper<HttpResponse>(this);
    }

    ObjectWrapper<HttpResponse> HttpResponse::setContentType(const HttpContentType& contentType){
        m_ContentType = contentType;
        m_Headers["Content-Type"] = HttpContentTypeToString(m_ContentType);
        return ObjectWrapper<HttpResponse>(this);
    }

    ObjectWrapper<HttpResponse> HttpResponse::setRange(uint64_t start,uint64_t stop,uint64_t totalLength){
        std::string value("bytes ");
        value.append(AsString<uint64_t>(start));
        value.append("-");
        value.append(AsString<uint64_t>(stop));
        value.append("/");
        value.append(AsString<uint64_t>(totalLength));
        m_Headers.emplace("Content-Range",value);
        return ObjectWrapper<HttpResponse>(this);
    }

    ObjectWrapper<HttpResponse> HttpResponse::setHeader(const std::string& key,const std::string& value){
        m_Headers.emplace(key,value);
        return ObjectWrapper<HttpResponse>(this);
    }

    ObjectWrapper<HttpResponse> HttpResponse::setCookie(const std::string& key,const std::string& val,std::time_t expired,const std::string& path,const std::string& domain,bool httpOnly,bool secure){
        std::string result;
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
            result.append(TimeToGMTString(expired));
        }
        if(httpOnly){
            result.append("; HttpOnly");
        }
        if(secure){
            result.append("; Secure");
        }
        m_Cookies.push_back(result);
        return ObjectWrapper<HttpResponse>(this);
    }

    std::ostream& operator<<(std::ostream& os,const Safe<HttpRequest>& request){
        return request->toStream(os);
    }

    std::ostream& operator<<(std::ostream& os,const Safe<HttpResponse>& response){
        return response->toStream(os);
    }
}
}
}
