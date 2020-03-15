#include "Http/WebSocket.h"
namespace Magic{
namespace Http{
    #pragma pack(1)
    struct WSFrame{
        uint32_t m_Code     :4;
        bool m_Rsv1         :1;
        bool m_Rsv2         :1;
        bool m_Rsv3         :1;
        bool m_Fin          :1;
        uint8_t m_Payload   :7;
        bool m_Mask         :1;
    };
    #pragma pack()

    WebSocket::WebSocket(uint64_t timeOutMs,Safe<WebSocketServletDispatch>& Servlet,asio::io_context& context)
        :Socket(timeOutMs,4096,context)
        ,m_Server(true)
        ,m_ServletDispatch(Servlet){
        m_RequestParser.reset(new Http::HttpRequestParser);
        m_ResponseParser.reset(new Http::HttpResponseParser);
    }
    void WebSocket::accept(){
        doRead([this](const std::error_code& ec){
                if(ec){
                if(ec == asio::error::eof || ec == asio::error::connection_reset || ec == asio::error::operation_aborted){
                    return;
                }
                MAGIC_LOG(LogLevel::LogWarn) << ec.message();
                return;
            }
            size_t nparse = m_RequestParser->execute(m_ReadBuffer.data(),m_ReadBuffer.size());
            if(m_RequestParser->hasError()) {
                MAGIC_LOG(LogLevel::LogDebug) << "HttpParser hasError";
                return;
            }
            uint64_t m_Offset = m_ReadBuffer.size() - nparse;
            m_ReadBuffer.resize(m_Offset);
            if(!m_RequestParser->isFinished()) {
                this->accept();
                return;
            }
            auto& request = m_RequestParser->getData();
            if(StringCompareNoCase(request->getHeader("Upgrade"), "WebSocket")) {
                return;
            }
            if(StringCompareNoCase(request->getHeader("Connection"), "Upgrade")) {
                return;
            }
            if(StringAs<uint32_t>(request->getHeader("Sec-webSocket-Version")) != 13) {
                return;
            }
            std::string key = request->getHeader("Sec-WebSocket-Key");
            if(key.empty()) {
                return;
            }
            std::stringstream sstream;
            auto& response = m_ResponseParser->getData();
            std::string val = key + "258EAFA5-E914-47DA-95CA-C5AB0DC85B11";
            val = Base64Encode(SHA1(val));
            response->setGzip(false);
            response->setWebSocket(true);
            response->setHeader("Upgrade", "websocket");
            response->setHeader("Connection", "Upgrade");
            response->setHeader("Sec-WebSocket-Accept", val);
            response->setStatus(Http::HttpStatus::SWITCHING_PROTOCOLS);
            sstream << response;
            const auto& buf = sstream.str();
            m_WriteBuffer.assign(buf.begin(),buf.end());
            doWrite([this](const std::error_code& ec){
                if(ec){
                    MAGIC_LOG(LogLevel::LogWarn) << ec.message();
                    return;
                }
                m_ServletDispatch->getServlet(m_Name)->onOpen();
                this->recvMessage();
            });
        });
    }

}
}