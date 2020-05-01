/*
 * @File: WebSocketMessage.cpp
 * @Author: INotFound
 * @Date: 2020-03-18 22:46:56
 * @LastEditTime: 2020-03-19 23:33:53
 */
#include "Http/WebSocketMessage.h"

namespace Magic{
namespace Http{
    WebSocketMessage::WebSocketMessage(const std::string& data,const WebSocketMessageType& type)
        :m_Data(data)
        ,m_Type(type){
    }
    WebSocketMessage::WebSocketMessage(const char* data,uint64_t size,const WebSocketMessageType& type)
        :m_Data(data,size)
        ,m_Type(type){
    }
    std::string& WebSocketMessage::getData(){
        return m_Data;
    }
    void WebSocketMessage::setData(const std::string& data){
        m_Data = data;
    }
    const WebSocketMessageType& WebSocketMessage::getType()const{
        return m_Type;
    }
    void WebSocketMessage::setType(const WebSocketMessageType& type){
        m_Type = type;
    }
}
}