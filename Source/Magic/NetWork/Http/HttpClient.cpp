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
    Safe<asio::io_context> g_IOService = std::make_shared<asio::io_context>();

    HttpClient::~HttpClient() {
        MAGIC_FATAL() << "~HttpClient";
    }

    HttpClient::HttpClient(const std::string& url,uint64_t timeOutMs)
        :m_Url(url)
        ,m_Death(false)
        ,m_Finish(false){
        m_Socket = std::make_shared<Socket>(timeOutMs,4096,*g_IOService);
        m_HttpSocket = std::make_shared<HttpSocket>(m_Socket);
        m_Socket->setHeartBeatCallBack([this](const Safe<Socket>& socket){
            if(m_Death){
                if(!m_Finish)
                    m_TimeOutCallBack();
                m_HttpSocket.reset();
                m_Socket.reset();
                socket->close();
                return;
            }
            m_Death = true;
            socket->runHeartBeat(this->shared_from_this());
        });
    }

    void HttpClient::execute(const Safe<HttpRequest>& request){
        Uri uri(m_Url);
        if(uri.hasError()){
            auto &errorCallBack = m_Socket->getErrorCodeCallBack();
            if (errorCallBack)
                errorCallBack(std::error_code(static_cast<int>(UriErrorCode::ParseError), Magic::NetWork::Http::UriErrorCategory()));;
            return;
        }

        uint16_t port = uri.getPort();
        const std::string& host = uri.getHost();
        const std::string& path = uri.getPath();
        const std::string& query = uri.getQuery();
        const std::string& fragment = uri.getFragment();

        if(!path.empty())
            request->setPath(path);
        if(!query.empty())
            request->setQuery(query);
        if(!fragment.empty())
            request->setFragment(fragment);
        if(!host.empty())
            request->setHeader("Host",host);

        asio::error_code errorCode;
        asio::ip::tcp::resolver resolver(*g_IOService);
        auto results = resolver.resolve(host, AsString(port),errorCode);
        if(errorCode){
            auto& errorCallBack = m_Socket->getErrorCodeCallBack();
            if(errorCallBack)
                errorCallBack(errorCode);
            return;
        }
        if(port == 443){
            asio::ssl::context sslContext(asio::ssl::context::sslv23);
            m_Socket->enableSsl(std::make_shared<asio::ssl::stream<asio::ip::tcp::socket&>>(*m_Socket->getEntity(),sslContext));
        }

        m_Socket->getEntity()->async_connect(results->endpoint(),[this,request](const asio::error_code& errorCode){
            if(errorCode) {
                if(errorCode == asio::error::eof || errorCode == asio::error::operation_aborted)
                    return;
                auto& errorCallBack = m_Socket->getErrorCodeCallBack();
                if(errorCallBack)
                    errorCallBack(errorCode);
                return;
            }
            m_Death = false;
            auto sslStream = m_Socket->getSslEntity();
            if(sslStream){
                sslStream->set_verify_mode(asio::ssl::verify_none);
                sslStream->async_handshake(asio::ssl::stream_base::client,[this,request](const asio::error_code& errorCode){
                    if(errorCode){
                        auto& errorCallBack = m_Socket->getErrorCodeCallBack();
                        if(errorCallBack)
                            errorCallBack(errorCode);
                        return;
                    }
                    m_Death = false;
                    m_Socket->getEntity()->set_option(asio::ip::tcp::no_delay(true));
                    m_HttpSocket->sendRequest(request);
                    m_HttpSocket->recvResponse([this](const Safe<HttpSocket>&,const Safe<HttpRequest>&,const Safe<HttpResponse>& httpResponse){
                        m_Death = true;
                        m_Finish = true;
                        m_Socket->close();
                        m_ResponseCallBack(httpResponse);
                    });
                });
                return;
            }
            m_Socket->getEntity()->set_option(asio::ip::tcp::no_delay(true));
            m_HttpSocket->sendRequest(request);
            m_HttpSocket->recvResponse([this](const Safe<HttpSocket>&,const Safe<HttpRequest>&,const Safe<HttpResponse>& httpResponse){
                m_Death = true;
                m_Finish = true;
                m_Socket->close();
                m_ResponseCallBack(httpResponse);
            });
        });
        m_Socket->runHeartBeat(this->shared_from_this());
        if(g_IOService->stopped())
            g_IOService->restart();
        g_IOService->run();
    }

    ObjectWrapper<HttpClient> HttpClient::onTimeOut(const std::function<void()>& callback) {
        m_TimeOutCallBack = std::move(callback);
        return ObjectWrapper<HttpClient>(this);
    }

    ObjectWrapper<HttpClient> HttpClient::onResponse(const std::function<void(const Safe<HttpResponse>&)>& callback){
        m_ResponseCallBack = std::move(callback);
        return ObjectWrapper<HttpClient>(this);
    }

}
}
}