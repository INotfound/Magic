/*
 * @file: 
 * @Author: INotFound
 * @Date: 2020-03-19 23:17:39
 * @LastEditTime: 2020-03-19 23:26:47
 */
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

    WebSocket::WebSocket(uint64_t timeOutMs,asio::io_context& context)
        :Socket(timeOutMs,4096,context)
        ,m_Server(true){
    }
}
}