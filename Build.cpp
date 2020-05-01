/*
 * @File: Build.cpp
 * @Author: INotFound
 * @Date: 2020-04-04 20:08:11
 * @LastEditTime: 2020-04-04 20:11:12
 */
//   Util
#include "Source/Magic.cpp"
#include "Source/Magic/Log.cpp"
#include "Source/Magic/Gzip.cpp"
#include "Source/Magic/Timer.cpp"
#include "Source/Magic/Crypto.cpp"
#include "Source/Magic/Thread.cpp"
#include "Source/Magic/Config.cpp"
#include "Source/Magic/Plugin.cpp"
#include "Source/Magic/IoPool.cpp"
#include "Source/Magic/Socket.cpp"
#include "Source/Magic/TcpServer.cpp"
#include "Source/Magic/TimingWheel.cpp"
//   DataBase
#include "Source/Magic/DB/MySql.cpp"
//   Http
#include "Source/Magic/Http/Http.cpp"
#include "Source/Magic/Http/Session.cpp"
#include "Source/Magic/Http/HttpFile.cpp"
#include "Source/Magic/Http/MultiPart.cpp"
#include "Source/Magic/Http/HttpCache.cpp"
#include "Source/Magic/Http/HttpSocket.cpp"
#include "Source/Magic/Http/HttpServer.cpp"
#include "Source/Magic/Http/HttpParser.cpp"
#include "Source/Magic/Http/HttpServlet.cpp"
// WebSocket
#include "Source/Magic/Http/WebSocket.cpp"
#include "Source/Magic/Http/WebSocketMessage.cpp"
#include "Source/Magic/Http/WebSocketServer.cpp"
//   Adapter
#include "Source/Magic/Adapter/WinUtil.cpp"
#include "Source/Magic/Adapter/WinMutex.cpp"
#include "Source/Magic/Adapter/WinPlugin.cpp"
#include "Source/Magic/Adapter/LinuxUtil.cpp"
#include "Source/Magic/Adapter/LinuxMutex.cpp"
#include "Source/Magic/Adapter/LinuxPlugin.cpp"
//   Ragel
#include "Source/Magic/Http/Util/Uri.rl.cpp"
#include "Source/Magic/Http/Util/http11_parser.rl.cpp"
#include "Source/Magic/Http/Util/httpclient_parser.rl.cpp"