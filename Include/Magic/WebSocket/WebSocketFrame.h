#pragma once
#include "Core.h"
namespace Magic{
namespace WebSocket{
namespace OPCODE{
    static const uint16_t CONTINUE    =0;
    static const uint16_t TEXT_FREAME =1;
    static const uint16_t BIN_FRAME   =2;
    static const uint16_t CLOSE       =8;
    static const uint16_t PING        =0x9;
    static const uint16_t PONG        =0xA;
}
    #pragma pack(1)
    struct WSFrame{
        bool m_Fin          :1;
        bool m_Rsv1         :1;
        bool m_Rsv2         :1;
        bool m_Rsv3         :1;
        uint32_t m_Code     :4;
        bool m_Mask         :1;
        uint8_t m_Payload   :7;
    };
    #pragma pack()
}
}