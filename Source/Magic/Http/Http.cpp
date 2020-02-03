#include "Http/Http.h"
#include <cstring>

namespace Magic{
namespace Http{

    static const char* g_MethodString[]{
        #define XX(num,name,string) #name,
            HTTP_METHOD_MAP(XX)
        #undef XX
    };
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
        uint64_t index = static_cast<uint64_t>(method);
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

    HttpContentType FileTypeToHttpContentType(const std::string& fileName){
        std::string extName;
        auto pos = fileName.rfind('.');
        if(pos != std::string::npos){
            extName = fileName.substr(pos+1);
            std::transform(extName.begin(),extName.end(),extName.begin(),tolower);
        }
        switch(extName.length()){
            case 0:
                return HttpContentType::APPLICATION_OCTET_STREAM;
            case 2:
                if(extName == "js"){
                    return HttpContentType::APPLICATION_X_JAVASCRIPT;
                }
                return HttpContentType::APPLICATION_OCTET_STREAM;
            case 3:
                switch(extName[0]){
                    case 'b':
                        if(extName == "bmp")
                            return HttpContentType::IMAGE_BMP;
                        break;
                    case 'c':
                        if(extName == "css")
                            return HttpContentType::TEXT_CSS;
                        break;
                    case 'e':
                        if(extName == "eot")
                            return HttpContentType::APPLICATION_VND_MS_FONTOBJ;
                        break;
                    case 'g':
                        if(extName == "gif")
                            return HttpContentType::IMAGE_GIF;
                        break;
                    case 'i':
                        if(extName == "ico")
                            return HttpContentType::IMAGE_XICON;
                        break;
                    case 'j':
                        if(extName == "jpg")
                            return HttpContentType::IMAGE_JPG;
                        break;
                    case 'o':
                        if(extName == "otf")
                            return HttpContentType::APPLICATION_X_FONT_OPENTYPE;
                        break;
                    case 'p':
                        if(extName == "png")
                            return HttpContentType::IMAGE_PNG;
                        break;
                    case 's':
                        if(extName == "svg")
                            return HttpContentType::IMAGE_SVG_XML;
                        break;
                    case 't':
                        if(extName == "txt")
                            return HttpContentType::TEXT_PLAIN;
                        else if(extName == "ttf")
                            return HttpContentType::APPLICATION_X_FONT_TRUETYPE;
                        break;
                    case 'x':
                        if(extName == "xml")
                            return HttpContentType::TEXT_XML;
                        else if(extName == "xsl")
                            return HttpContentType::TEXT_XSL;
                        break;
                    default:
                        break;
                }
                return HttpContentType::APPLICATION_OCTET_STREAM;
            case 4:
                if(extName == "html")
                    return HttpContentType::TEXT_HTML;
                else if(extName == "jpeg")
                    return HttpContentType::IMAGE_JPG;
                else if(extName == "icns")
                    return HttpContentType::IMAGE_ICNS;
                else if(extName == "woff")
                    return HttpContentType::APPLICATION_FONT_WOFF;
                return HttpContentType::APPLICATION_OCTET_STREAM;
            case 5:
                if(extName == "woff2")
                    return HttpContentType::APPLICATION_FONT_WOFF2;
                return HttpContentType::APPLICATION_OCTET_STREAM;
            default:
                return HttpContentType::APPLICATION_OCTET_STREAM;
        }
    }

    const char* HttpContentTypeToString(const HttpContentType& contentType){
        switch(contentType){
            #define XX(name,desc) \
            case HttpContentType::name: \
                return desc;
            HTTP_CONTENT_TYPE(XX);
            #undef XX
            default:
                return "<unknown>";
        }
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
    void HttpRequest::setkeepAlive(bool keepAlive){
        m_KeepAlive = keepAlive;
    }
    void HttpRequest::setBody(const std::string& body){
        m_Body = body;
    }
    void HttpRequest::setQuery(const std::string& query){
        m_Query = query;
    }
    void HttpRequest::setUrlPath(const std::string& urlPath){
        m_UrlPath = urlPath;
    }
    void HttpRequest::setFragment(const std::string& fragment){
        m_Fragment = fragment;
    }
    
    void HttpRequest::setParams(const KeyValue& v){
        m_Params = v;
    }
    void HttpRequest::setHeaders(const KeyValue& v){
        m_Headers = v;
    }

    void HttpRequest::setParam(const std::string& key,const std::string& value){
        m_Params.emplace(key,value);
    }

    void HttpRequest::setHeader(const std::string& key,const std::string& value){
        m_Headers.emplace(key, value);
    }

    bool HttpRequest::getkeepAlive() const{
        return m_KeepAlive;
    }
    uint8_t HttpRequest::getVersion() const{
        return m_Version;
    }
    HttpMethod HttpRequest::getMethod() const{
        return m_Method;
    }
    const std::string& HttpRequest::getBody() const{
        return m_Body;
    }        
    const std::string& HttpRequest::getQuery() const{
        return m_Query;
    }        
    const std::string& HttpRequest::getUrlPath() const{
        return m_UrlPath;
    }

    HttpRequest::KeyValue& HttpRequest::getParams() {
        return m_Params;
    }
    HttpRequest::KeyValue& HttpRequest::getHeaders() {
        return m_Headers;
    }

    bool HttpRequest::hasParam(const std::string& key,std::string& value){
        auto iter = m_Params.find(key);
        if(iter == m_Params.end()){
            return false;
        }
        value.assign(iter->second);
        return true;
    }
    bool HttpRequest::hasHeader(const std::string& key,std::string& value){
        auto iter = m_Headers.find(key);
        if(iter == m_Headers.end()){
            return false;
        }
        value.assign(iter->second);
        return true;
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
            << "\r\n";

        os << "Connection: " << (m_KeepAlive ? "keep-alive" : "close") << "\r\n";
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
    void HttpResponse::setkeepAlive(bool keepAlive){
        m_KeepAlive = keepAlive;
    }
    void HttpResponse::setBody(const std::string& body){
        m_Body = body;
    }
    void HttpResponse::setReason(const std::string& reason){
        m_Reason = reason;
    }
    void HttpResponse::setContentType(const HttpContentType contentType){
        m_ContentType = contentType;
    }
    void HttpResponse::setHeader(const std::string& key,const std::string& value){
        m_Headers.emplace(key,value);
    }

    bool HttpResponse::getkeepAlive() const{
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

    bool HttpResponse::hasHeader(const std::string& key,std::string& value){
        auto iter = m_Headers.find(key);
        if(iter == m_Headers.end()){
            return false;
        }
        value.assign(iter->second);
        return true;
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
        os << "Connection: " << (m_KeepAlive ? "keep-alive" : "close") << "\r\n";
        os << "Content-Type: " << HttpContentTypeToString(m_ContentType) << "\r\n";
        for(auto& v : m_Headers){
            if(StringCompareNoCase(v.first,"Connection") == 0 || StringCompareNoCase(v.first,"Content-Type")){
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