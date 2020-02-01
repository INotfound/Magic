#pragma once
#include <map>
#include <string>
#include <ostream>
#include "Core.h"

namespace Magic{
namespace Http{



/* Request Methods */
    #define HTTP_METHOD_MAP(XX)         \
        XX(0,  DELETE,      DELETE)       \
        XX(1,  GET,         GET)          \
        XX(2,  HEAD,        HEAD)         \
        XX(3,  POST,        POST)         \
        XX(4,  PUT,         PUT)          \
        /* pathological */                \
        XX(5,  CONNECT,     CONNECT)      \
        XX(6,  OPTIONS,     OPTIONS)      \
        XX(7,  TRACE,       TRACE)        \
        /* WebDAV */                      \
        XX(8,  COPY,        COPY)         \
        XX(9,  LOCK,        LOCK)         \
        XX(10, MKCOL,       MKCOL)        \
        XX(11, MOVE,        MOVE)         \
        XX(12, PROPFIND,    PROPFIND)     \
        XX(13, PROPPATCH,   PROPPATCH)    \
        XX(14, SEARCH,      SEARCH)       \
        XX(15, UNLOCK,      UNLOCK)       \
        XX(16, BIND,        BIND)         \
        XX(17, REBIND,      REBIND)       \
        XX(18, UNBIND,      UNBIND)       \
        XX(19, ACL,         ACL)          \
        /* subversion */                  \
        XX(20, REPORT,      REPORT)       \
        XX(21, MKACTIVITY,  MKACTIVITY)   \
        XX(22, CHECKOUT,    CHECKOUT)     \
        XX(23, MERGE,       MERGE)        \
        /* upnp */                        \
        XX(24, MSEARCH,     M-SEARCH)     \
        XX(25, NOTIFY,      NOTIFY)       \
        XX(26, SUBSCRIBE,   SUBSCRIBE)    \
        XX(27, UNSUBSCRIBE, UNSUBSCRIBE)  \
        /* RFC-5789 */                    \
        XX(28, PATCH,       PATCH)        \
        XX(29, PURGE,       PURGE)        \
        /* CalDAV */                      \
        XX(30, MKCALENDAR,  MKCALENDAR)   \
        /* RFC-2068, section 19.6.1.2 */  \
        XX(31, LINK,        LINK)         \
        XX(32, UNLINK,      UNLINK)       \
        /* icecast */                     \
        XX(33, SOURCE,      SOURCE)       \

    /* Status Codes */
    #define HTTP_STATUS_MAP(XX)                                                 \
        XX(100, CONTINUE,                        Continue)                        \
        XX(101, SWITCHING_PROTOCOLS,             Switching Protocols)             \
        XX(102, PROCESSING,                      Processing)                      \
        XX(200, OK,                              OK)                              \
        XX(201, CREATED,                         Created)                         \
        XX(202, ACCEPTED,                        Accepted)                        \
        XX(203, NON_AUTHORITATIVE_INFORMATION,   Non-Authoritative Information)   \
        XX(204, NO_CONTENT,                      No Content)                      \
        XX(205, RESET_CONTENT,                   Reset Content)                   \
        XX(206, PARTIAL_CONTENT,                 Partial Content)                 \
        XX(207, MULTI_STATUS,                    Multi-Status)                    \
        XX(208, ALREADY_REPORTED,                Already Reported)                \
        XX(226, IM_USED,                         IM Used)                         \
        XX(300, MULTIPLE_CHOICES,                Multiple Choices)                \
        XX(301, MOVED_PERMANENTLY,               Moved Permanently)               \
        XX(302, FOUND,                           Found)                           \
        XX(303, SEE_OTHER,                       See Other)                       \
        XX(304, NOT_MODIFIED,                    Not Modified)                    \
        XX(305, USE_PROXY,                       Use Proxy)                       \
        XX(307, TEMPORARY_REDIRECT,              Temporary Redirect)              \
        XX(308, PERMANENT_REDIRECT,              Permanent Redirect)              \
        XX(400, BAD_REQUEST,                     Bad Request)                     \
        XX(401, UNAUTHORIZED,                    Unauthorized)                    \
        XX(402, PAYMENT_REQUIRED,                Payment Required)                \
        XX(403, FORBIDDEN,                       Forbidden)                       \
        XX(404, NOT_FOUND,                       Not Found)                       \
        XX(405, METHOD_NOT_ALLOWED,              Method Not Allowed)              \
        XX(406, NOT_ACCEPTABLE,                  Not Acceptable)                  \
        XX(407, PROXY_AUTHENTICATION_REQUIRED,   Proxy Authentication Required)   \
        XX(408, REQUEST_TIMEOUT,                 Request Timeout)                 \
        XX(409, CONFLICT,                        Conflict)                        \
        XX(410, GONE,                            Gone)                            \
        XX(411, LENGTH_REQUIRED,                 Length Required)                 \
        XX(412, PRECONDITION_FAILED,             Precondition Failed)             \
        XX(413, PAYLOAD_TOO_LARGE,               Payload Too Large)               \
        XX(414, URI_TOO_LONG,                    URI Too Long)                    \
        XX(415, UNSUPPORTED_MEDIA_TYPE,          Unsupported Media Type)          \
        XX(416, RANGE_NOT_SATISFIABLE,           Range Not Satisfiable)           \
        XX(417, EXPECTATION_FAILED,              Expectation Failed)              \
        XX(421, MISDIRECTED_REQUEST,             Misdirected Request)             \
        XX(422, UNPROCESSABLE_ENTITY,            Unprocessable Entity)            \
        XX(423, LOCKED,                          Locked)                          \
        XX(424, FAILED_DEPENDENCY,               Failed Dependency)               \
        XX(426, UPGRADE_REQUIRED,                Upgrade Required)                \
        XX(428, PRECONDITION_REQUIRED,           Precondition Required)           \
        XX(429, TOO_MANY_REQUESTS,               Too Many Requests)               \
        XX(431, REQUEST_HEADER_FIELDS_TOO_LARGE, Request Header Fields Too Large) \
        XX(451, UNAVAILABLE_FOR_LEGAL_REASONS,   Unavailable For Legal Reasons)   \
        XX(500, INTERNAL_SERVER_ERROR,           Internal Server Error)           \
        XX(501, NOT_IMPLEMENTED,                 Not Implemented)                 \
        XX(502, BAD_GATEWAY,                     Bad Gateway)                     \
        XX(503, SERVICE_UNAVAILABLE,             Service Unavailable)             \
        XX(504, GATEWAY_TIMEOUT,                 Gateway Timeout)                 \
        XX(505, HTTP_VERSION_NOT_SUPPORTED,      HTTP Version Not Supported)      \
        XX(506, VARIANT_ALSO_NEGOTIATES,         Variant Also Negotiates)         \
        XX(507, INSUFFICIENT_STORAGE,            Insufficient Storage)            \
        XX(508, LOOP_DETECTED,                   Loop Detected)                   \
        XX(510, NOT_EXTENDED,                    Not Extended)                    \
        XX(511, NETWORK_AUTHENTICATION_REQUIRED, Network Authentication Required) \

    enum class HttpMethod{
        #define XX(num,name,string) name = num,
            HTTP_METHOD_MAP(XX)
        #undef XX
        INVALID_METHOD = 255
    };

    enum class HttpStatus{
        #define XX(code,name,desc) name = code,
            HTTP_STATUS_MAP(XX)
        #undef XX
        INVALID_METHOD = 255
    };
    HttpMethod CharsToHttpMethod(const char* str);
    HttpMethod StringToHttpMethod(const std::string &str);
    const char* HttpMethodToString(const HttpMethod & method);
    const char* HttpStatusToString(const HttpStatus & status);
    class CaseInsensitiveLess{
    public:
        bool operator()(const std::string&, const std::string&) const;
    };
    class HttpRequest{
    public:
        typedef std::map<std::string,std::string> KeyValue;
    public:
        HttpRequest(bool keepAlive = true,uint8_t version = 0x11);
        void setVersion(uint8_t ver);
        void setMethod(HttpMethod method);
        void setkeepAlive(bool keepAlive);
        void setBody(const std::string& body);
        void setQuery(const std::string& query);
        void setUrlPath(const std::string& urlPath);
        void setFragment(const std::string& fragment);
        
        void setParams(const KeyValue& val);
        void setCookies(const KeyValue& val);
        void setHeaders(const KeyValue& val);

        void setParam(const std::string& key,const std::string& value);
        void setCookie(const std::string& key,const std::string& value);
        void setHeader(const std::string& key,const std::string& value);

        bool getkeepAlive() const;
        uint8_t getVersion() const;
        HttpMethod getMethod() const;
        const std::string& getBody() const;
        const std::string& getQuery() const;
        const std::string& getUrlPath() const;

        KeyValue& getParams();
        KeyValue& getCookies();
        KeyValue& getHeaders(); 

        bool hasParam(const std::string& key,std::string& value);
        bool hasCookie(const std::string& key,std::string& value);
        bool hasHeader(const std::string& key,std::string& value);

        void delParam(const std::string& key);
        void delCookie(const std::string& key);
        void delHeader(const std::string& key);

        std::ostream& toStream(std::ostream& os);
    private:
        bool m_KeepAlive{};
        uint8_t m_Version{};
        HttpMethod m_Method{};
        std::string m_Body{};
        std::string m_Query{};
        std::string m_UrlPath{};
        std::string m_Fragment{};
        KeyValue m_Params{};
        KeyValue m_Cookies{};
        KeyValue m_Headers{};
    };

    class HttpResponse{
    public:
        typedef std::map<std::string,std::string,CaseInsensitiveLess> KeyValue;
    public:
        HttpResponse(bool keepAlive = true,uint8_t version = 0x11);
        void setVersion(uint8_t ver);
        void setStatus(HttpStatus status);
        void setkeepAlive(bool keepAlive);
        void setBody(const std::string& body);
        void setReason(const std::string& reason);

//        void setCookies(const KeyValue& val);
        void setHeaders(const KeyValue& val);

//        void setCookie(const std::string& key,const std::string& value);
        void setHeader(const std::string& key,const std::string& value);

        bool getkeepAlive() const;
        uint8_t getVersion() const;
        HttpStatus getStatus() const;
        const std::string& getBody() const;
        const std::string& getReason() const;

        // KeyValue& getCookies();
        KeyValue& getHeaders(); 

        // bool hasCookie(const std::string& key,std::string& value);
        bool hasHeader(const std::string& key,std::string& value);

        // void delCookie(const std::string& key);
        void delHeader(const std::string& key);

        std::ostream& toStream(std::ostream& os);
    private:
        bool m_KeepAlive{};
        uint8_t m_Version{};
        std::string m_Body{};
        std::string m_Reason{};
        KeyValue m_Headers{};
        HttpStatus m_Status{}; 
        std::vector<std::string> m_Cookies;
    };
    std::ostream& operator<<(std::ostream& os, const MagicPtr<HttpResponse>& request);
    std::ostream& operator<<(std::ostream& os, const MagicPtr<HttpResponse>& response);
}
}