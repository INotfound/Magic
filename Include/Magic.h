/*
 * @file: Magic.h
 * @Author: INotFound
 * @Date: 2020-03-13 20:55:19
 * @LastEditTime: 2020-03-13 20:57:16
 */
#pragma once
//Util
#include "Magic/Log.h"
#include "Magic/Any.h"
#include "Magic/Core.h"
#include "Magic/Gzip.h"
#include "Magic/Timer.h"
#include "Magic/Macro.h"
#include "Magic/Mutex.h"
#include "Magic/Crypto.h"
#include "Magic/IoPool.h"
#include "Magic/Thread.h"
#include "Magic/Socket.h"
#include "Magic/Config.h"
#include "Magic/Plugin.h"
#include "Magic/Adapter.h"
#include "Magic/TcpServer.h"
#include "Magic/TimingWheel.h"
//DataBase
#include "Magic/DB/MySql.h"
//Http
#include "Magic/Http/Uri.h"
#include "Magic/Http/Http.h"
#include "Magic/Http/Session.h"
#include "Magic/Http/HttpFile.h"
#include "Magic/Http/HttpCache.h"
#include "Magic/Http/MultiPart.h"
#include "Magic/Http/HttpServer.h"
#include "Magic/Http/HttpSocket.h"
#include "Magic/Http/HttpParser.h"
#include "Magic/Http/HttpServlet.h"

namespace Magic {
    void Init(const std::string& Name);
}
