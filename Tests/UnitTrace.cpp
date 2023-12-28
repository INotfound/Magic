#include "gtest/gtest.h"
#include "Magic/Utilty/Trace.hpp"

void testFunc(){
    for(auto i = 0;i<100;i++){
        MAGIC_TRACE_FUNCTION();
    }
}

TEST(MagicTraceTest,Trace){
    Magic::g_TraceAppender = std::make_shared<Magic::ChromiumTraceAppender>("trace.json");
    testFunc();
    Magic::g_TraceAppender->complete();
}
