#include "Http/WebSocketMessage.h"

namespace Magic{
namespace Http{
    WebSocketMessage::WebSocketMessage(const OpCode& opCode,const std::string& data)
        :m_Code(opCode)
        ,m_Data(data){
    }
    std::string& WebSocketMessage::getData(){
        return m_Data;
    }
    const OpCode& WebSocketMessage::getOpCode()const{
        return m_Code;
    }
    void WebSocketMessage::setOpCode(const OpCode& opcode){
        m_Code = opcode;
    }
    void WebSocketMessage::setData(const std::string& data){
        m_Data = data;
    }
}
}