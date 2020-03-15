#pragma once
#include "Core.h"
namespace Magic{
namespace Http{
    enum class OpCode{
        CONTINUE        =0,
        TEXT_FREAME     =1,
        BIN_FRAME       =2,
        CLOSE           =8,
        PING            =0x9,
        PONG            =0xA
    };
    class WebSocketMessage{
    public:
        WebSocketMessage(const OpCode& opCode,const std::string& data ="");
        std::string& getData();
        const OpCode& getOpCode()const;
        void setData(const std::string& data);
        void setOpCode(const OpCode& opcode);
    private:
        OpCode m_Code;
        std::string m_Data;
    };
}
}