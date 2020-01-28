#include "Http/Http.h"
#include <cstring>
#include "Util.h"

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

    bool CaseInsensitiveLess::operator()(const std::string& lhs,const std::string& rhs) const{
        return CompareNoCase(lhs,rhs) < 0;
    }

    HttpRequest::HttpRequest(bool keepAlive,uint8_t version)
        :m_KeepAlive{keepAlive}
        ,m_Version{version}
        ,m_UrlPath{"/"}{
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
    void HttpRequest::setCookies(const KeyValue& v){
        m_Cookies = v;
    }
    void HttpRequest::setHeaders(const KeyValue& v){
        m_Headers = v;
    }

    void HttpRequest::setParam(const std::string& key,const std::string& value){
        m_Params[key] = value;
    }
    void HttpRequest::setCookie(const std::string& key,const std::string& value){
        m_Cookies[key] = value;
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
    HttpRequest::KeyValue& HttpRequest::getCookies() {
        return m_Cookies;
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
    bool HttpRequest::hasCookie(const std::string& key,std::string& value){
        auto iter = m_Cookies.find(key);
        if(iter == m_Cookies.end()){
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
    void HttpRequest::delCookie(const std::string& key){
        m_Cookies.erase(key);
    }
    void HttpRequest::delHeader(const std::string& key){
        m_Headers.erase(key);
    }
    std::ostream& HttpRequest::dump(std::ostream& os){
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
            if(CompareNoCase(v.first,"Connection")){
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
        :m_KeepAlive{keepAlive}
        ,m_Version{version}
        ,m_Status{HttpStatus::OK}{

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

    void HttpResponse::setCookies(const KeyValue& val){
        m_Cookies = val;
    }
    void HttpResponse::setHeaders(const KeyValue& val){
        m_Headers = val;
    }
    void HttpResponse::setCookie(const std::string& key,const std::string& value){
        m_Cookies[key] = value;
    }
    void HttpResponse::setHeader(const std::string& key,const std::string& value){
        m_Headers[key] = value;
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

    HttpResponse::KeyValue& HttpResponse::getCookies(){
        return m_Cookies;
    }
    HttpResponse::KeyValue& HttpResponse::getHeaders(){
        return m_Headers;
    }

    bool HttpResponse::hasCookie(const std::string& key,std::string& value){
        auto iter = m_Cookies.find(key);
        if(iter == m_Cookies.end()){
            return false;
        }
        value.assign(iter->second);
        return true;
    }
    bool HttpResponse::hasHeader(const std::string& key,std::string& value){
        auto iter = m_Headers.find(key);
        if(iter == m_Headers.end()){
            return false;
        }
        value.assign(iter->second);
        return true;
    }

    void HttpResponse::delCookie(const std::string& key){
        m_Cookies.erase(key);
    }
    void HttpResponse::delHeader(const std::string& key){
        m_Headers.erase(key);
    }

    std::ostream& HttpResponse::dump(std::ostream& os){
        os  << "HTTP/"
            << static_cast<uint32_t>(m_Version >> 4)
            << '.'
            << static_cast<uint32_t>(m_Version & 0x0F)
            << ' '
            << static_cast<uint32_t>(m_Status)
            << ' '
            << (m_Reason.empty() ? HttpStatusToString(m_Status) : m_Reason)
            << "\r\n";
        for(auto& v : m_Headers){
            if(CompareNoCase(v.first,"Connection")){
                continue;
            }
            os << v.first << ": " << v.second << "\r\n";
        }
        if(!m_Body.empty()){
            os  << "Content-Length: " << m_Body.size() << "\r\n\r\n"
                << m_Body;
        }else{
            os << "\r\n";
        }
        return os;
    }

}
}