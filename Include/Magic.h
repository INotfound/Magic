#pragma once
#include "Log.h"
#include "Any.h"
#include "Md5.h"
#include "Core.h"
#include "Gzip.h"
#include "Macro.h"
#include "Mutex.h"
#include "IoPool.h"
#include "Thread.h"
#include "Config.h"
#include "Plugin.h"
#include "Adapter.h"
#include "TcpServer.h"
#include "Http/Http.h"
#include "Http/Session.h"
#include "Http/HttpFile.h"
#include "Http/MultiPart.h"
#include "Http/HttpParser.h"
#include "Http/HttpServer.h"
#include "Http/HttpServlet.h"

namespace Magic {
    void Init(const std::string& Name);
}
