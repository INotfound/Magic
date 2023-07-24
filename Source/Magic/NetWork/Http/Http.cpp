/*
 ******************************************************************************
 * @Author         : INotfound
 * @File           : Http.cpp
 * @Date           : 2023-07-03 18:32
 ******************************************************************************
 */
#include <algorithm>

#include "Magic/Utilty/Compress.hpp"
#include "Magic/NetWork/Http/Http.hpp"

namespace Magic{
namespace NetWork{
namespace Http{
    static const StringView g_MethodString[]{
        #define XX(num,name,string) #string,
            HTTP_METHOD_MAP(XX)
        #undef XX
    };

    static const std::map<StringView,HttpMethod,CaseResponsiveLess> g_HttpMethod = {
        #define XX(num,name,string) {#string,HttpMethod::name},
            HTTP_METHOD_MAP(XX)
        #undef XX
    };

    static const std::map<StringView,HttpContentType,CaseInsensitiveLess> g_HttpContent = {
        #define XX(name,extName,desc){extName,HttpContentType::name},
            HTTP_CONTENT_TYPE(XX)
        #undef XX
    };

    static const std::map<HttpContentType,StringView> g_HttpContentType = {
        #define XX(name,extName,desc){HttpContentType::name,desc},
            HTTP_CONTENT_TYPE(XX)
        #undef XX
    };

    bool IsUrlEncode(const StringView& str){
        return str.find('%') != std::string::npos || str.find('+') != std::string::npos;
    }

    HttpMethod StringToHttpMethod(const StringView& str){
        auto iter = g_HttpMethod.find(str);
        if(iter != g_HttpMethod.end()){
            return iter->second;
        }
        return HttpMethod::InvalidMethod;
    }

    StringView HttpMethodToString(const HttpMethod& method){
        auto index = static_cast<uint64_t>(method);
        if(index >= (sizeof(g_MethodString) / sizeof(g_MethodString[0]))){
            return StringView("<unknown>");
        }
        return g_MethodString[index];
    }

    StringView HttpStatusToString(const HttpStatus& status){
        switch(status){
        #define XX(code,name,desc)  \
            case HttpStatus::name : \
                return StringView(#desc);
            HTTP_STATUS_MAP(XX)
        #undef XX
            default:
                return StringView("<unknown>");
        }
    }

    std::string UrlEncode(const StringView& value) noexcept{
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

    std::string UrlDecode(const StringView& value) noexcept{
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

    HttpContentType FileTypeToHttpContentType(const StringView& fileName){
        auto pos = fileName.rfind('.');
        if(pos != StringView::npos){
            StringView sv = fileName.substr(pos + 1);
            auto iter = g_HttpContent.find(sv);
            if(iter != g_HttpContent.end()){
                return iter->second;
            }
        }
        return HttpContentType::ApplicationOctetStream;
    }

    const char* HttpContentTypeToString(const HttpContentType& contentType){
        return g_HttpContentType.at(contentType).data();
    }

    template<typename Map>
    inline void Parse(const StringView& str,Map& map,const StringView& flag){
        uint64_t pos = 0;
        do{
            uint64_t idx = 0;
            StringView rawString = SubString(str,pos,flag);
            pos += static_cast<uint64_t>(rawString.size() + 1);
            if(IsUrlEncode(rawString)){
                std::string decodeString = UrlDecode(rawString);
                idx = decodeString.find('=');
                if(idx == std::string::npos)
                    break;
                map.emplace(decodeString.substr(0,idx),decodeString.substr(idx + 1));
            }else{
                idx = rawString.find('=');
                if(idx == std::string::npos)
                    break;
                StringView svKey = rawString.substr(0,idx);
                StringView svValue = rawString.substr(idx + 1);
                map.emplace(std::string(svKey.data(),svKey.size()),std::string(svValue.data(),svValue.size()));
            }
        }while(pos <= str.size());
    }

    template<typename Map>
    inline void ParseCookies(const StringView& str,Map& map,const StringView& flag){
        uint64_t pos = 0;
        do{
            uint64_t key = 0;
            StringView rawString = SubString(str,pos,flag);
            pos += static_cast<uint64_t>(rawString.size() + 1);
            if(IsUrlEncode(rawString)){
                std::string decodeString = UrlDecode(rawString);
                key = decodeString.find('=');
                if(key == std::string::npos)
                    break;
                map.emplace(Trim(decodeString.substr(0,key)),Trim(decodeString.substr(key + 1)));
            }else{
                key = rawString.find('=');
                if(key == std::string::npos)
                    break;
                map.emplace(Trim(rawString.substr(0,key)),Trim(rawString.substr(key + 1)));
            }
        }while(pos <= str.size());
    }

    HttpRequest::HttpRequest(bool keepAlive,uint8_t version)
        :m_KeepAlive(keepAlive)
        ,m_Version(version)
        ,m_Method(HttpMethod::Get)
        ,m_UrlPath("/")
        ,m_ContentLength(0){
    }

    bool HttpRequest::isRange() const{
        return m_Headers.find("Range") != m_Headers.end();
    }

    bool HttpRequest::hasBody() const{
        return m_BodyStream.operator bool();
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

    StringView HttpRequest::getPath() const{
        return m_UrlPath;
    }

    StringView HttpRequest::getBody() const{
        if(m_BodyStream){
            m_BodyStream->seek(0);
            return m_BodyStream->read();
        }else{
            return StringView();
        }
    }

    StringView HttpRequest::getQuery() const{
        return m_Query;
    }

    StringView HttpRequest::getCookie(const StringView& key){
        if(!m_Cookies.empty()){
            auto value = m_Cookies.find(std::string(key.data(),key.size()));
            if(value == m_Cookies.end()){
                return g_EmptyString;
            }
            return value->second;
        }
        auto iter = m_Headers.find("Cookie");
        if(iter == m_Headers.end())
            return g_EmptyString;
        ParseCookies(iter->second,m_Cookies,";");
        auto value = m_Cookies.find(std::string(key.data(),key.size()));
        if(value == m_Cookies.end()){
            return g_EmptyString;
        }
        return value->second;
    }

    StringView HttpRequest::getParam(const StringView& key) const{
        auto iter = m_Params.find(std::string(key.data(),key.size()));
        if(iter == m_Params.end()){
            return g_EmptyString;
        }
        return iter->second;
    }

    StringView HttpRequest::getHeader(const StringView& key) const{
        auto iter = m_Headers.find(std::string(key.data(),key.size()));
        if(iter == m_Headers.end()){
            return g_EmptyString;
        }
        return iter->second;
    }

    void HttpRequest::delParam(const StringView& key){
        m_Params.erase(std::string(key.data(),key.size()));
    }

    void HttpRequest::delHeader(const StringView& key){
        m_Headers.erase(std::string(key.data(),key.size()));
    }

    void HttpRequest::toStream(std::ostream& os){
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

        if(m_BodyStream){
            std::string body;
            m_BodyStream->seek(0);
            while(!m_BodyStream->eof()){
                auto buffer = m_BodyStream->read();
                body.append(buffer.data(),buffer.data());
            }
            os  << "Content-Length: " << body.size() << "\r\n\r\n"
                << body;
        }else{
            if(m_ContentLength != 0){
                os << "Content-Length: " << m_ContentLength << "\r\n";
            }
            os << "\r\n";
        }
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

    ObjectWrapper<HttpRequest> HttpRequest::setContentLength(uint64_t length){
        m_ContentLength = length;
        return ObjectWrapper<HttpRequest>(this);
    }

    ObjectWrapper<HttpRequest> HttpRequest::setRange(uint64_t start,uint64_t stop){
        m_Headers["Range"] = StringCat("bytes="
                                       ,AsString<uint64_t>(start)
                                       ,"-"
                                       ,(stop == 0) ? "" : AsString<uint64_t>(stop));
        return ObjectWrapper<HttpRequest>(this);
    }

    ObjectWrapper<HttpRequest> HttpRequest::setBody(const StringView& body){
        auto contentType = m_Headers.find("Content-Type");
        if(contentType != m_Headers.end() && StringCompareNoCase(contentType->second,"application/x-www-form-urlencoded")){
            Parse(body,m_Params,"&");
        }
        m_BodyStream = std::make_shared<DataStream>(body);
        return ObjectWrapper<HttpRequest>(this);
    }

    ObjectWrapper<HttpRequest> HttpRequest::setBody(const Safe<IStream>& bodyStream){
        m_BodyStream = bodyStream;
        return ObjectWrapper<HttpRequest>(this);
    }

    ObjectWrapper<HttpRequest> HttpRequest::setQuery(const StringView& query){
        Parse(query,m_Params,"&");
        m_Query = std::string(query.data(),query.size());
        return ObjectWrapper<HttpRequest>(this);
    }

    ObjectWrapper<HttpRequest> HttpRequest::setPath(const StringView& urlPath){
        m_UrlPath = std::string(urlPath.data(),urlPath.size());
        return ObjectWrapper<HttpRequest>(this);
    }

    ObjectWrapper<HttpRequest> HttpRequest::setFragment(const StringView& fragment){
        m_Fragment = std::string(fragment.data(),fragment.size());
        return ObjectWrapper<HttpRequest>(this);
    }

    ObjectWrapper<HttpRequest> HttpRequest::setParam(const StringView& key,const StringView& value){
        m_Params[std::string(key.data(),key.size())] = std::string(value.data(),value.size());
        return ObjectWrapper<HttpRequest>(this);
    }

    ObjectWrapper<HttpRequest> HttpRequest::setHeader(const StringView& key,const StringView& value){
        m_Headers[std::string(key.data(),key.size())] = std::string(value.data(),value.size());
        return ObjectWrapper<HttpRequest>(this);
    }

    ObjectWrapper<HttpRequest> HttpRequest::setCookie(const StringView& key,const StringView& value){
        m_Cookies[std::string(key.data(),key.size())] = std::string(value.data(),value.size());
        return ObjectWrapper<HttpRequest>(this);
    }

    HttpResponse::HttpResponse(bool keepAlive,uint8_t version)
        :m_KeepAlive(keepAlive)
        ,m_Version(version)
        ,m_Status(HttpStatus::Ok)
        ,m_ContentLength(0)
        ,m_ContentType(HttpContentType::TextPlain){
    }

    bool HttpResponse::isRange() const{
        return m_Headers.find("Content-Range") != m_Headers.end();
    }

    bool HttpResponse::hasBody() const{
        return m_BodyStream.operator bool();
    }

    bool HttpResponse::hasResource() const{
        return !m_Resource.empty();
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

    StringView HttpResponse::getBody() const{
        if(m_BodyStream){
            m_BodyStream->seek(0);
            return m_BodyStream->read();
        }else{
            return StringView();
        }
    }

    uint64_t HttpResponse::getContentLength() const{
        return m_ContentLength;
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

    StringView HttpResponse::getReason() const{
        return m_Reason;
    }

    StringView HttpResponse::getResource() const{
        return m_Resource;
    }

    HttpContentType HttpResponse::getContentType() const{
        return m_ContentType;
    }

    void HttpResponse::delHeader(const StringView& key){
        m_Headers.erase(std::string(key.data(),key.size()));
    }

    StringView HttpResponse::getHeader(const StringView& key){
        auto iter = m_Headers.find(std::string(key.data(),key.size()));
        if(iter == m_Headers.end()){
            return g_EmptyString;
        }
        return iter->second;
    }

    void HttpResponse::toStream(std::ostream& os){
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

        bool hasBody = m_BodyStream.operator bool();
        auto encodingIter = m_Headers.find("Content-Encoding");
        if(encodingIter != m_Headers.end()){
            const auto& contentEncoding = encodingIter->second;
            if(hasBody && contentEncoding.find("br") != StringView::npos){
                encodingIter->second = "br";
                m_BodyStream = std::make_shared<BrotliEncoder>(m_BodyStream);
            }
        #ifdef ZLIB
            else if(hasBody && contentEncoding.find("gzip") != StringView::npos){
                encodingIter->second = "gzip";
                m_BodyStream = std::make_shared<GZipEncoder>(m_BodyStream);
            }
        #endif
            else{
                m_Headers.erase(encodingIter);
            }
        }

        for(auto& v : m_Headers){
            os << v.first << ": " << v.second << "\r\n";
        }

        for(auto& v : m_Cookies){
            os << "Set-Cookie: " << v << "\r\n";
        }

        if(hasBody){
            std::string body;
            m_BodyStream->seek(0);
            while(!m_BodyStream->eof()){
                auto buffer = m_BodyStream->read();
                body.append(buffer.data(),buffer.size());
            }
            os  << "Content-Length: " << body.size() << "\r\n\r\n"
                << body;
        }else{
            if(m_ContentLength != 0){
                os << "Content-Length: " << m_ContentLength << "\r\n";
            }
            os << "\r\n";
        }
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

    ObjectWrapper<HttpResponse> HttpResponse::setContentLength(uint64_t length){
        m_ContentLength = length;
        return ObjectWrapper<HttpResponse>(this);
    }

    ObjectWrapper<HttpResponse> HttpResponse::setBody(const StringView& body){
        m_BodyStream = std::make_shared<DataStream>(body);
        return ObjectWrapper<HttpResponse>(this);
    }

    ObjectWrapper<HttpResponse> HttpResponse::setBody(const Safe<IStream>& bodyStream){
        m_BodyStream = bodyStream;
        return ObjectWrapper<HttpResponse>(this);
    }

    ObjectWrapper<HttpResponse> HttpResponse::setReason(const StringView& reason){
        m_Reason = std::string(reason.data(),reason.size());
        return ObjectWrapper<HttpResponse>(this);
    }

    ObjectWrapper<HttpResponse> HttpResponse::setResource(const StringView& filePath){
        m_Resource = std::string(filePath.data(),filePath.size());
        return ObjectWrapper<HttpResponse>(this);
    }

    ObjectWrapper<HttpResponse> HttpResponse::setContentType(const StringView& contentType){
        m_Headers["Content-Type"] = std::string(contentType.data(),contentType.size());
        return ObjectWrapper<HttpResponse>(this);
    }

    ObjectWrapper<HttpResponse> HttpResponse::setContentType(const HttpContentType& contentType){
        m_ContentType = contentType;
        m_Headers["Content-Type"] = HttpContentTypeToString(m_ContentType);
        return ObjectWrapper<HttpResponse>(this);
    }

    ObjectWrapper<HttpResponse> HttpResponse::setRange(uint64_t start,uint64_t stop,uint64_t totalLength){
        m_Headers["Content-Range"] = StringCat("bytes "
                                               ,AsString<uint64_t>(start)
                                               ,"-"
                                               ,AsString<uint64_t>(stop)
                                               ,"/"
                                               ,AsString<uint64_t>(totalLength));
        return ObjectWrapper<HttpResponse>(this);
    }

    ObjectWrapper<HttpResponse> HttpResponse::setHeader(const StringView& key,const StringView& value){
        m_Headers[std::string(key.data(),key.size())] = std::string(value.data(),value.size());
        return ObjectWrapper<HttpResponse>(this);
    }

    ObjectWrapper<HttpResponse> HttpResponse::setCookie(const StringView& key,const StringView& val,std::time_t expired,const StringView& path,const StringView& domain,bool httpOnly,bool secure){
        std::string result = StringCat(key,"=",val);
        if(!path.empty()){
            result.append("; Path=");
            result.append(path.data(),path.size());
        }
        if(!domain.empty()){
            result.append("; Domain=");
            result.append(domain.data(),domain.size());
        }
        if(expired > 0){
            result.append("; Expires=");
            result.append(TimeToString(expired,"%a, %d %b %Y %H:%M:%S %Z"));
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
}
}
}
