/*
 * @file: WebSoscketMessage.h
 * @Author: INotFound
 * @Date: 2020-03-18 22:50:40
 * @LastEditTime: 2020-03-18 22:58:43
 */
#pragma once
#include "Core.h"
namespace Magic{
namespace Http{
    enum class WebSocketMessageType{
        CONTINUE    = 0,
        TEXT        = 1,
        CLOSE       = 8,
        PING        = 9,
        PONG        = 0xA
    };
    class WebSocketMessage{
    public:
        WebSocketMessage(const std::string& data,const WebSocketMessageType& type);
        WebSocketMessage(const char* data,uint64_t size,const WebSocketMessageType& type);
        std::string& getData();
        void setData(const std::string& data);
        const WebSocketMessageType& getType()const;
        void setType(const WebSocketMessageType& type);
    private:
        std::string m_Data;
        WebSocketMessageType m_Type;
    };
}
}