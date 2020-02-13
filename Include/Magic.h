#pragma once
#include "Magic/Log.h"
#include "Magic/Any.h"
#include "Magic/Md5.h"
#include "Magic/Core.h"
#include "Magic/Gzip.h"
#include "Magic/Macro.h"
#include "Magic/Mutex.h"
#include "Magic/IoPool.h"
#include "Magic/Thread.h"
#include "Magic/Config.h"
#include "Magic/Plugin.h"
#include "Magic/Adapter.h"
#include "Magic/TcpServer.h"
#include "Magic/Http/Http.h"
#include "Magic/Http/Session.h"
#include "Magic/Http/HttpFile.h"
#include "Magic/Http/MultiPart.h"
#include "Magic/Http/HttpParser.h"
#include "Magic/Http/HttpServer.h"
#include "Magic/Http/HttpServlet.h"

namespace Magic {
    void Init(const std::string& Name);
}
