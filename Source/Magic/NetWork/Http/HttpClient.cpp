/*
 * @Author: INotFound
 * @Date: 2022-07-30 22:11:54
 * @LastEditTime: 2022-07-30 23:17:50
 */
#include "Magic/Utilty/Logger.h"
#include "Magic/NetWork/IoPool.h"
#include "Magic/NetWork/Http/HttpClient.h"

namespace Magic{
namespace NetWork{
namespace Http{
    HttpClient::~HttpClient() = default;

    HttpClient::HttpClient(const std::string_view& url,uint64_t timeOutMs)
        :m_Url(url.data(),url.size())
        ,m_Death(false)
        ,m_Finish(false){
        if(g_IoPool){
            m_IOService = g_IoPool->get();
        }else{
            throw Failure("HttpClient Cannot Be Created Because IoPool Not Call ExternMode");
        }
        m_Socket = std::make_shared<Socket>(timeOutMs,4096,*m_IOService);
        m_Socket->setHeartBeatCallBack([this](const Safe<Socket>& socket){
            if(m_Death){
                if(m_TimeOutCallBack && !m_Finish)
                    m_TimeOutCallBack();
                socket->close();
                return;
            }
            m_Death = true;
            socket->runHeartBeat(this->shared_from_this());
        });
    }

    void HttpClient::execute(Safe<HttpRequest>&& request){
        Uri uri(m_Url);
        if(uri.hasError()){
            auto& errorCallBack = m_Socket->getErrorCodeCallBack();
            if(errorCallBack)
                errorCallBack(std::error_code(static_cast<int>(UriErrorCode::ParseError),Magic::NetWork::Http::UriErrorCategory()));;
            return;
        }

        uint16_t port = uri.getPort();
        std::string_view host = uri.getHost();
        std::string_view path = uri.getPath();
        std::string_view query = uri.getQuery();
        std::string_view fragment = uri.getFragment();

        if(!path.empty())
            request->setPath(path);
        if(!query.empty())
            request->setQuery(query);
        if(!fragment.empty())
            request->setFragment(fragment);
        if(!host.empty())
            request->setHeader("Host",host);

        asio::error_code errorCode;
        asio::ip::tcp::resolver resolver(*m_IOService);
        auto results = resolver.resolve(std::string(host.data(),host.size()),AsString(port),errorCode);
        if(errorCode){
            auto& errorCallBack = m_Socket->getErrorCodeCallBack();
            if(errorCallBack)
                errorCallBack(errorCode);
            return;
        }
    #ifdef OPENSSL
        if(port == 443){
            m_Socket->enableSsl();
        }
    #endif
        auto self = this->shared_from_this();
    #if __cplusplus >= 201402L
        m_Socket->getEntity()->async_connect(results->endpoint(),[this,self = std::move(self),request = std::move(request)](const asio::error_code& errorCode){
    #else
        m_Socket->getEntity()->async_connect(results->endpoint(),[this,self,request](const asio::error_code& errorCode){
    #endif
            if(errorCode){
                if(errorCode == asio::error::eof || errorCode == asio::error::operation_aborted)
                    return;
                auto& errorCallBack = m_Socket->getErrorCodeCallBack();
                if(errorCallBack)
                    errorCallBack(errorCode);
                return;
            }
        #ifdef OPENSSL
            auto sslStream = m_Socket->getSslEntity();
            if(sslStream){
                sslStream->set_verify_mode(asio::ssl::verify_none);
            #if __cplusplus >= 201402L
                sslStream->async_handshake(asio::ssl::stream_base::client,[this,self = std::move(const_cast<Safe<HttpClient>&>(self)),request = std::move(const_cast<Safe<HttpRequest>&>(request))](const asio::error_code& errorCode){
            #else
                sslStream->async_handshake(asio::ssl::stream_base::client,[this,self,request](const asio::error_code& errorCode){
            #endif
                    if(errorCode){
                        auto& errorCallBack = m_Socket->getErrorCodeCallBack();
                        if(errorCallBack)
                            errorCallBack(errorCode);
                        return;
                    }
                    m_Socket->getEntity()->set_option(asio::ip::tcp::no_delay(true));
                    Safe<HttpSocket> httpSocket = std::make_shared<HttpSocket>(m_Socket);
                    httpSocket->sendRequest(request);
                #if __cplusplus >= 201402L
                    httpSocket->recvResponse([this,self = std::move(const_cast<Safe<HttpClient>&>(self))](const Safe<HttpSocket>& socket){
                #else
                    httpSocket->recvResponse([this,self](const Safe<HttpSocket>& socket){
                #endif
                        m_Finish = true;
                        m_Socket->close();
                        m_ResponseCallBack(socket->getResponse());
                    });
                });
                return;
            }
        #endif
            m_Socket->getEntity()->set_option(asio::ip::tcp::no_delay(true));
            Safe<HttpSocket> httpSocket = std::make_shared<HttpSocket>(m_Socket);
            httpSocket->sendRequest(request);
        #if __cplusplus >= 201402L
            httpSocket->recvResponse([this,self = std::move(const_cast<Safe<HttpClient>&>(self))](const Safe<HttpSocket>& socket){
        #else
            httpSocket->recvResponse([this,self](const Safe<HttpSocket>& socket){
        #endif
                m_Finish = true;
                m_Socket->close();
                if(m_ResponseCallBack)
                    m_ResponseCallBack(socket->getResponse());
            });
        });
        m_Socket->runHeartBeat(self);
    }

    ObjectWrapper<HttpClient> HttpClient::onTimeOut(std::function<void()> callBack){
        m_TimeOutCallBack = std::move(callBack);
        return ObjectWrapper<HttpClient>(this);
    }

    ObjectWrapper<HttpClient> HttpClient::onError(std::function<void(const asio::error_code&)> callBack){
    #if __cplusplus >= 201402L
        m_Socket->setErrorCodeCallBack([this,callBack = std::move(callBack)](const asio::error_code& errorCode){
    #else
        m_Socket->setErrorCodeCallBack([this,callBack](const asio::error_code& errorCode){
    #endif
            m_Finish = true;
            callBack(errorCode);
        });
        return ObjectWrapper<HttpClient>(this);
    }

    ObjectWrapper<HttpClient> HttpClient::onResponse(std::function<void(const Safe<HttpResponse>&)> callBack){
        m_ResponseCallBack = std::move(callBack);
        return ObjectWrapper<HttpClient>(this);
    }

}
}
}