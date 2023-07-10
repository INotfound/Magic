/*
 ******************************************************************************
 * @Author         : INotfound
 * @File           : Http.hpp
 * @Date           : 2023-07-03 18:27
 ******************************************************************************
 */
#pragma once

#include <map>
#include <ostream>

#include "Magic/Core/Core.hpp"
#include "Magic/Utilty/String.hpp"

namespace Magic{
namespace NetWork{
namespace Http{
    /* Request Methods */
    #undef DELETE
    #define HTTP_METHOD_MAP(XX)           \
        XX(0,  Delete,      DELETE)       \
        XX(1,  Get,         GET)          \
        XX(2,  Head,        HEAD)         \
        XX(3,  Post,        POST)         \
        XX(4,  Put,         PUT)          \
        /* pathological */                \
        XX(5,  Connect,     CONNECT)      \
        XX(6,  Options,     OPTIONS)      \
        XX(7,  Trace,       TRACE)        \
        /* WebDAV */                      \
        XX(8,  Copy,        COPY)         \
        XX(9,  Lock,        LOCK)         \
        XX(10, MKCol,       MKCOL)        \
        XX(11, Move,        MOVE)         \
        XX(12, PropFind,    PROPFIND)     \
        XX(13, PropPatch,   PROPPATCH)    \
        XX(14, Search,      SEARCH)       \
        XX(15, UnLock,      UNLOCK)       \
        XX(16, Bind,        BIND)         \
        XX(17, ReBind,      REBIND)       \
        XX(18, UnBind,      UNBIND)       \
        XX(19, Acl,         ACL)          \
        /* subversion */                  \
        XX(20, RePort,      REPORT)       \
        XX(21, MKActivity,  MKACTIVITY)   \
        XX(22, CheckOut,    CHECKOUT)     \
        XX(23, Merge,       MERGE)        \
        /* upnp */                        \
        XX(24, MSearch,     M-SEARCH)     \
        XX(25, Notify,      NOTIFY)       \
        XX(26, SubScribe,   SUBSCRIBE)    \
        XX(27, UnSubScribe, UNSUBSCRIBE)  \
        /* RFC-5789 */                    \
        XX(28, Patch,       PATCH)        \
        XX(29, Purge,       PURGE)        \
        /* CalDAV */                      \
        XX(30, MKCalendar,  MKCALENDAR)   \
        /* RFC-2068, section 19.6.1.2 */  \
        XX(31, Link,        LINK)         \
        XX(32, UnLink,      UNLINK)       \
        /* icecast */                     \
        XX(33, Source,      SOURCE)       \

    /* Status Codes */
    #define HTTP_STATUS_MAP(XX)                                                   \
        XX(100, Continue,                        Continue)                        \
        XX(101, SwitchingProtocols,              Switching Protocols)             \
        XX(102, Processing,                      Processing)                      \
        XX(200, Ok,                              OK)                              \
        XX(201, Created,                         Created)                         \
        XX(202, Accepted,                        Accepted)                        \
        XX(203, NonAuthoritativeInformation,     Non-Authoritative Information)   \
        XX(204, NoContent,                       No Content)                      \
        XX(205, ResetContent,                    Reset Content)                   \
        XX(206, PartialContent,                  Partial Content)                 \
        XX(207, MultiStatus,                     Multi-Status)                    \
        XX(208, AlreadyReported,                 Already Reported)                \
        XX(226, ImUsed,                          IM Used)                         \
        XX(300, MultipleChoices,                 Multiple Choices)                \
        XX(301, MovedPermanently,                Moved Permanently)               \
        XX(302, Found,                           Found)                           \
        XX(303, SeeOther,                        See Other)                       \
        XX(304, NotModified,                     Not Modified)                    \
        XX(305, UseProxy,                        Use Proxy)                       \
        XX(307, TemporaryRedirect,               Temporary Redirect)              \
        XX(308, PermanentRedirect,               Permanent Redirect)              \
        XX(400, BadRequest,                      Bad Request)                     \
        XX(401, UnAuthorized,                    Unauthorized)                    \
        XX(402, PaymentRequired,                 Payment Required)                \
        XX(403, ForBidden,                       Forbidden)                       \
        XX(404, NotFound,                        Not Found)                       \
        XX(405, MethodNotAllowed,                Method Not Allowed)              \
        XX(406, NotAcceptable,                   Not Acceptable)                  \
        XX(407, ProxyAuthenticationRequired,     Proxy Authentication Required)   \
        XX(408, RequestTimeout,                  Request Timeout)                 \
        XX(409, Conflict,                        Conflict)                        \
        XX(410, Gone,                            Gone)                            \
        XX(411, LengthRequired,                  Length Required)                 \
        XX(412, PreConditionFailed,              Precondition Failed)             \
        XX(413, PayloadTooLarge,                 Payload Too Large)               \
        XX(414, UriTooLong,                      URI Too Long)                    \
        XX(415, UnSupportedMediaType,            Unsupported Media Type)          \
        XX(416, RangeNotSatisfiable,             Range Not Satisfiable)           \
        XX(417, ExpectationFailed,               Expectation Failed)              \
        XX(421, MisdirectedRequest,              Misdirected Request)             \
        XX(422, UnProcessableEntity,             Unprocessable Entity)            \
        XX(423, Locked,                          Locked)                          \
        XX(424, FailedDependency,                Failed Dependency)               \
        XX(426, UpgradeRequired,                 Upgrade Required)                \
        XX(428, PreConditionRequired,            Precondition Required)           \
        XX(429, TooManyRequests,                 Too Many Requests)               \
        XX(431, RequestHeaderFieldsTooLarge,     Request Header Fields Too Large) \
        XX(451, UnavailableForLegalReasons,      Unavailable For Legal Reasons)   \
        XX(500, InternalServerError,             Internal Server Error)           \
        XX(501, NotImplemented,                  Not Implemented)                 \
        XX(502, BadGateWay,                      Bad Gateway)                     \
        XX(503, ServiceUnAvailable,              Service Unavailable)             \
        XX(504, GateWayTimeOut,                  Gateway Timeout)                 \
        XX(505, HttpVersionNotSupported,         HTTP Version Not Supported)      \
        XX(506, VariantAlsoNegotiates,           Variant Also Negotiates)         \
        XX(507, InSufficientStorage,             Insufficient Storage)            \
        XX(508, LoopDetected,                    Loop Detected)                   \
        XX(510, NotExtended,                     Not Extended)                    \
        XX(511, NetWorkAuthenticationRequired,   Network Authentication Required) \

    /* Content Type */
    #define HTTP_CONTENT_TYPE(XX)                                                               \
        XX(TextCss,                    "css",      "text/css; charset=utf-8")                  \
        XX(TextXml,                    "xml",      "text/xml; charset=utf-8")                  \
        XX(TextXsl,                    "xsl",      "text/xsl; charset=utf-8")                  \
        XX(TextHtml,                   "html",     "text/html; charset=utf-8")                 \
        XX(TextPlain,                  "txt",      "text/plain; charset=utf-8")                \
        XX(ApplicationXml,             "xml",      "application/xml; charset=utf-8")           \
        XX(ApplicationJson,            "json",     "application/json; charset=utf-8")          \
        XX(ApplicationForm,            "",         "application/x-www-form-urlencoded")        \
        XX(ApplicationJavascript,      "js",       "application/javascript; charset=utf-8")    \
        XX(ApplicationOctetStream,     "",         "application/octet-stream")                 \
        XX(ImagePng,                   "png",      "image/png")                                \
        XX(ImageJpg,                   "jpg",      "image/jpeg")                               \
        XX(ImageJpeg,                  "jpeg",     "image/jpeg")                               \
        XX(ImageGif,                   "gif",      "image/gif")                                \
        XX(ImageBmp,                   "bmp",      "image/bmp")                                \
        XX(ImageWebp,                  "webp",     "image/webp")                               \
        XX(ImageXicon,                 "ico",      "image/icon")                               \
        XX(ImageSvgXml,                "svg",      "image/svg+xml")                            \
        XX(ApplicationWasm,            "wasm",     "application/wasm")                         \
        XX(ApplicationFontWoff,        "woff",     "application/font-woff")                    \
        XX(ApplicationFontWoff2,       "woff2",    "application/font-woff2")                   \
        XX(ApplicationFontTrueType,    "ttf",      "application/x-font-truetype")              \
        XX(ApplicationFontOpenType,    "otf",      "application/x-font-opentype")              \
        XX(ApplicationVndMsFontObj,    "eot",      "application/vnd.ms-fontobject")            \

    /**
     * @brief HttpMethod
     */
    enum class HttpMethod{
        #define XX(num,name,string) name = (num),
            HTTP_METHOD_MAP(XX)
        #undef XX
        InvalidMethod = 255
    };
    /**
     * @brief HttpStatus
     */
    enum class HttpStatus{
        #define XX(code,name,desc) name = (code),
            HTTP_STATUS_MAP(XX)
        #undef XX
        InvalidMethod = 255
    };
    /**
     * @brief HttpContentType
     */
    enum class HttpContentType{
        #define XX(name,extName,desc) name,
            HTTP_CONTENT_TYPE(XX)
        #undef XX
    };

    /**
     * @brief 是否是Url的编码
     * @param str 需要检查的字符串
     * @return: 返回True则是，返回False则是否
     */
    bool IsUrlEncode(const Magic::StringView& str);

    /**
     * @brief 将String转换成HttpMethod
     * @param str 需要转换的字符串
     * @return: 返回HttpMethod
     */
    HttpMethod StringToHttpMethod(const Magic::StringView& str);

    /**
     * @brief 将HttpMethod转换成String
     * @param method 需要转换的HttpMethod
     * @return: 返回转换后的String
     */
    Magic::StringView HttpMethodToString(const HttpMethod& method);

    /**
     * @brief 将HttpStatus转换成String
     * @param status 需要转换的HttpStatus
     * @return: 返回转换后的String
     */
    Magic::StringView HttpStatusToString(const HttpStatus& status);

    /**
     * @brief 进行Url编码
     * @param value 需要编码的字符串
     * @return: 返回编码后的String
     */
    std::string UrlEncode(const Magic::StringView& value) noexcept;

    /**
     * @brief 进行Url解码
     * @param value 需要解码的字符串
     * @return: 返回解码的String
     */
    std::string UrlDecode(const Magic::StringView& value) noexcept;

    /**
     * @brief 将FileType转换成HttpContentType
     * @param fileName 需要转换的文件名称
     * @return: 返回ttpContentType
     */
    HttpContentType FileTypeToHttpContentType(const Magic::StringView& fileName);
    /**
     * @brief Http请求类
     */
    class HttpRequest:public Noncopyable{
    public:
        typedef std::map<std::string,std::string,CaseInsensitiveLess> KeyValue;
    public:
        /**
         * @brief 构造函数
         * @param keepAlive 是否长连接
         * @param version Http协议版本
         */
        explicit HttpRequest(bool keepAlive = true,uint8_t version = 0x11);
        /**
         * @brief 是否Range切片
         * @return: 返回True则是，返回False则是否
         */
        bool isRange() const;

        /**
         * @brief 获取是否为长连接函数
         * @return: 返回True则是，返回False则是否
         */
        bool getKeepAlive() const;

        /**
         * @brief 获取版本号函数
         * @return: 返回版本号
         */
        uint8_t getVersion() const;

        /**
         * @brief 获取HttpMethod函数
         * @return: 返回HttpMethod类型
         */
        HttpMethod getMethod() const;

        /**
         * @brief 获取切片结束位置函数
         * @return: 返回切片结束位置
         */
        uint64_t getRangeStop() const;

        /**
         * @brief 获取切片开始位置函数
         * @return: 返回切片开始位置
         */
        uint64_t getRangeStart() const;

        /**
         * @brief 获取正文长度函数
         * @return: 返回正文长度
         */
        uint64_t getContentLength() const;

        /**
         * @brief 获取路径函数
         * @return: 返回路径字符串
         */
        Magic::StringView getPath() const;

        /**
         * @brief 获取主体正文函数
         * @return: 返回主体正文字符串
         */
        Magic::StringView getBody() const;

        /**
         * @brief 获取查询函数
         * @return: 返回查询字符串
         */
        Magic::StringView getQuery() const;

        /**
         * @brief 获取Cookie函数
         * @param key 指定的键
         * @return: 返回根据指定的键对应的值
         */
        Magic::StringView getCookie(const Magic::StringView& key);

        /**
         * @brief 获取参数函数
         * @param key 指定的键
         * @return: 返回根据指定的键对应的值
         */
        Magic::StringView getParam(const Magic::StringView& key) const;

        /**
         * @brief 获取头参数函数
         * @param key 指定的键 不区分大小写
         * @return: 返回根据指定的键对应的值
         */
        Magic::StringView getHeader(const Magic::StringView& key) const;

        /**
         * @brief 获取输出流函数
         * @param os 输出流
         */
        void toStream(std::ostream& os);

        /**
         * @brief 删除参数函数
         * @param key 指定的键
         */
        void delParam(const Magic::StringView& key);

        /**
         * @brief 删除头参数函数
         * @param key 指定的键
         */
        void delHeader(const Magic::StringView& key);

        /**
         * @brief 设置版本号函数
         * @param ver 版本号
         */
        ObjectWrapper<HttpRequest> setVersion(uint8_t ver);

        /**
         * @brief 设置HttpMethod函数
         * @param method HttpMethod类型
         */
        ObjectWrapper<HttpRequest> setMethod(HttpMethod method);

        /**
         * @brief 设置是否为长连接函数
         * @param keepAlive 是否为长连接
         */
        ObjectWrapper<HttpRequest> setKeepAlive(bool keepAlive);

        /**
         * @brief 设置主体正文长度函数
         * @param length 正文长度
         */
        ObjectWrapper<HttpRequest> setContentLength(uint64_t length);

        /**
         * @brief 设置切片范围
         * @param start 切片开始范围
         * @param stop 切片结束范围
         */
        ObjectWrapper<HttpRequest> setRange(uint64_t start,uint64_t stop);

        /**
         * @brief 设置主体正文函数
         * @param body 主体正文
         */
        ObjectWrapper<HttpRequest> setBody(const Magic::StringView& body);

        /**
         * @brief 设置查询字符串函数
         * @param query 查询字符串
         */
        ObjectWrapper<HttpRequest> setQuery(const Magic::StringView& query);

        /**
         * @brief 设置Url路径函数
         * @param urlPath Url路径
         */
        ObjectWrapper<HttpRequest> setPath(const Magic::StringView& urlPath);

        /**
         * @brief 设置片段函数
         * @param fragment 片段
         */
        ObjectWrapper<HttpRequest> setFragment(const Magic::StringView& fragment);

        /**
         * @brief 设置参数函数
         * @param key 键
         * @param value 值
         */
        ObjectWrapper<HttpRequest> setParam(const Magic::StringView& key,const Magic::StringView& value);

        /**
         * @brief 设置Http头中的参数函数
         * @param key 键
         * @param value 值
         */
        ObjectWrapper<HttpRequest> setHeader(const Magic::StringView& key,const Magic::StringView& value);

        /**
         * @brief 设置Cookie函数
         * @param key 键
         * @param value 值
         */
        ObjectWrapper<HttpRequest> setCookie(const Magic::StringView& key,const Magic::StringView& value);

    private:
        bool m_KeepAlive;
        uint8_t m_Version;
        KeyValue m_Params;
        KeyValue m_Headers;
        KeyValue m_Cookies;
        HttpMethod m_Method;
        std::string m_Body;
        std::string m_Query;
        std::string m_UrlPath;
        std::string m_Fragment;
        uint64_t m_ContentLength;
    };

    /**
     * @brief Http响应类
     */
    class HttpResponse:public Noncopyable{
    public:
        typedef std::map<std::string,std::string,CaseInsensitiveLess> KeyValue;
    public:
        /**
         * @brief 构造函数
         * @param keepAlive 是否长连接
         * @param version 版本号
         */
        explicit HttpResponse(bool keepAlive = true,uint8_t version = 0x11);

        /**
         * @brief 是否Range切片
         * @return: 返回True则是，返回False则是否
         */
        bool isRange() const;

        /**
         * @brief 是否拥有资源
         * @return: 返回True则是，返回False则是否
         */
        bool hasResource() const;

        /**
         * @brief 获取是否为长连接函数
         * @return: 返回是否为长连接
         */
        bool getKeepAlive() const;

        /**
         * @brief 获取版本号函数
         * @return: 返回版本号
         */
        uint8_t getVersion() const;

        /**
         * @brief 获取切片结束位置函数
         * @return: 返回切片结束位置
         */
        uint64_t getRangeStop() const;

        /**
         * @brief 获取HttpStatus函数
         * @return: 返回HttpStatus类型
         */
        HttpStatus getStatus() const;

        /**
         * @brief 获取切片开始位置函数
         * @return: 返回切片开始位置
         */
        uint64_t getRangeStart() const;

        /**
         * @brief 获取正文长度函数
         * @return: 返回正文长度
         */
        uint64_t getContentLength() const;

        /**
         * @brief 获取主体正文函数
         * @return: 返回主体正文
         */
        Magic::StringView getBody() const;

        /**
         * @brief 获取切片总共长度函数
         * @return: 返回切片总共长度位置
         */
        uint64_t getRangeTotalLength() const;

        /**
         * @brief 获取响应解释函数
         * @return: 返回响应解释
         */
        Magic::StringView getReason() const;

        /**
         * @brief 获取资源函数
         * @return: 返回资源路径
         */
        Magic::StringView getResource() const;

        /**
         * @brief 获取正文类型
         * @return 返回正文类型
         */
        HttpContentType getContentType() const;

        /**
         * @brief 删除Http头键值对函数
         * @param key 指定的键
         */
        void delHeader(const Magic::StringView& key);

        /**
         * @brief 获取Http头键值对函数
         * @param key 指定的键
         * @return: 返回根据指定的键对应的值
         */
        Magic::StringView getHeader(const Magic::StringView& key);

        /**
         * @brief 获取输出流函数
         * @param os 输出流
         */
        void toStream(std::ostream& os);

        /**
         * @brief 设置版本号函数
         * @param ver 版本号
         */
        ObjectWrapper<HttpResponse> setVersion(uint8_t ver);

        /**
         * @brief 设置HttpStatus函数
         * @param status HttpStatus类型
         */
        ObjectWrapper<HttpResponse> setStatus(HttpStatus status);

        /**
         * @brief 设置是否为长连接函数
         * @param keepAlive 是否为长连接
         */
        ObjectWrapper<HttpResponse> setKeepAlive(bool keepAlive);

        /**
         * @brief 设置主体正文长度函数
         * @param length 主体正文长度
         */
        ObjectWrapper<HttpResponse> setContentLength(uint64_t length);

        /**
         * @brief 设置主体正文函数
         * @param body 主体正文
         */
        ObjectWrapper<HttpResponse> setBody(const Magic::StringView& body);

        /**
         * @brief 设置响应解释函数
         * @param reason 解释
         */
        ObjectWrapper<HttpResponse> setReason(const Magic::StringView& reason);

        /**
         * @brief 设置资源函数
         * @param filePath 资源路径
         */
        ObjectWrapper<HttpResponse> setResource(const Magic::StringView& filePath);

        /**
         * @brief 设置正文类型函数
         * @param contentType 正文类型
         */
        ObjectWrapper<HttpResponse> setContentType(const Magic::StringView& contentType);

        /**
         * @brief 设置正文类型函数
         * @param contentType 正文类型
         */
        ObjectWrapper<HttpResponse> setContentType(const HttpContentType& contentType);

        /**
         * @brief 设置切片范围
         * @param start 切片开始范围
         * @param stop 切片结束范围
         * @param totalLength 切片总长度
         */
        ObjectWrapper<HttpResponse> setRange(uint64_t start,uint64_t stop,uint64_t totalLength);

        /**
         * @brief 设置Http头键值对函数
         * @param key 键
         * @param value 值
         */
        ObjectWrapper<HttpResponse> setHeader(const Magic::StringView& key,const Magic::StringView& value);

        /**
         * @brief 设置Cookie函数
         * @param key 键
         * @param val 值
         * @param expired 过期时间
         * @param path Cookie路径
         * @param domain Cookie范围
         * @param httpOnly 是否HttpOnly
         * @param secure 是否Secure
         */
        ObjectWrapper<HttpResponse> setCookie(const Magic::StringView& key,const Magic::StringView& val,std::time_t expired = 0,const Magic::StringView& path = "",const Magic::StringView& domain = "",bool httpOnly = true,bool secure = false);

    private:
        bool m_KeepAlive;
        uint8_t m_Version;
        KeyValue m_Headers;
        std::string m_Body;
        HttpStatus m_Status;
        std::string m_Reason;
        std::string m_Resource;
        uint64_t m_ContentLength;
        HttpContentType m_ContentType;
        std::vector<std::string> m_Cookies;
    };

    inline void operator<<(std::ostream& os,HttpRequest& request){
        request.toStream(os);
    }

    inline void operator<<(std::ostream& os,HttpResponse& response){
        response.toStream(os);
    }

    inline void operator<<(std::ostream& os,const Safe<HttpRequest>& request){
        request->toStream(os);
    }

    inline void operator<<(std::ostream& os,const Safe<HttpResponse>& response){
        response->toStream(os);
    }
}
}
}
