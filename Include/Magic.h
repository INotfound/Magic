/*
 * @Author: INotFound
 * @Date: 2020-09-12 16:35:42
 * @LastEditTime: 2021-01-08 13:48:15
 */
#pragma once
#include "Core/Any.h"
#include "Core/Core.h"
#include "Core/Timer.h"
#include "Core/Mutex.h"
#include "Core/Logger.h"
#include "Core/Config.h"
#include "Core/Container.h"
#include "Core/TimingWheel.h"

#include "Utilty/TimeUtils.h"
#include "Utilty/MutexUtils.h"
#include "Utilty/ThreadUtils.h"
#include "Utilty/StringUtils.h"
#include "Utilty/CryptoUtils.h"

#include "NetWork/IoPool.h"
#include "NetWork/TcpServer.h"
#include "NetWork/Http/HttpParser.h"
#include "NetWork/Http/HttpSocket.h"
#include "NetWork/Http/HttpServer.h"
#include "NetWork/Http/HttpServlet.h"
#include "NetWork/Http/HttpSession.h"

#include "DataBase/ConnectionPool.h"

namespace Magic{
    const Safe<Container>& Configure(const std::function<void(const Safe<Container>&)>& configure);
}