<!--
 * @file: README.md
 * @Author: INotFound
 * @Date: 2020-03-16 00:33:44
 * @LastEditTime: 2020-04-11 16:50:55
 -->
# Magic服务端基础框架
* Magic 是一个能Web快速开发,且基于ASIO网络框架,拥有丰富的基础组件(Win And Linux)跨平台框架库。
## 快速示例
* 本库将智能指针名称进行了重定义.
```
    /// 文件 Magic/Include/Magic/Core.h 中
    #define Safe std::unique_ptr
    #define Share std::shared_ptr
```
* 在Main函数的第一行调用Magic的初始化方法,进行Config、Log相关组件的初始化工作.
```
    /// "Build_Test" 是项目名称.
    Magic::Init("Build_Test");
```
* Config配置组件的食用方法,目前只支持基础类型.
```
    /// 可自定义配置生成Json 和 ini格式的文件.具体常考 (Magic/Source/Magic.cpp) 文件。
    /// 未修改生成的配置文件的情况下,会使用at函数的第二个参数作为默认值.
    uint64_t number = Magic::Config::GetInstance()->at<uint64_t>("number", 24);
```
* Log日志组件的食用方法.
```
    /// 可自定义配置生成Html格式日志文件和普通日志文件.
    /// 可通过Magic::LogLevel枚举类调整日志输出等级.
    MAGIC_LOG(Magic::LogLevel::LogDebug) << "hello world";
```
* Plugin插件组件的食用方法.
```
    /// 详见以下两文件 
    -> Examples/Test_Plugin.cpp
    -> Examples/Test_PluginLib.cpp
```
* 简单的Web服务器的食用方法.
```
    /// 详见以下文件 
    -> Examples/Test_Server.cpp
```
## 如何编译
### 编译器的支持
* 只需支持C++11的编译器.
### 需要添加依赖的第三方库
* 需初始化更新Git的子仓库(Asio、zlib、RapidJson).
```
    /// 在此Git仓库的根路径下调用以下命令。
    git submodule update --init
```
* 加密库 OpenSSL ,数据库 MySql ,压缩库 zlib .
## 引用方法之一
* 推荐使用CMake进行构建.
* 配置MySql | OpenSSL (Linux可直接通过包管理器安装,Win需要下载编译安装) .
```
    //修改本库中的Library.cmake，将其变量指向你编译后的地址 (Linux无需如此).
########################################################################################
    #   Include
    set(OPENSSL_INCLUDE "C:\\Users\\magic\\Desktop\\openssl\\ssl\\include")
    set(MYSQL_INCLUDE   "C:\\Program Files (x86)\\MySQL\\MySQL Connector C 6.1\\include")
    #   Library
    set(OPENSSL_LIBRARY "C:\\Users\\magic\\Desktop\\openssl\\ssl\\lib")
    set(MYSQL_LIBRARY   "C:\\Program Files (x86)\\MySQL\\MySQL Connector C 6.1\\lib")
########################################################################################
```
* 把以下代码添加进你的项目文件CMakeLists.txt中进行后即可进行构建编译.
```
########################################################################################
set(MAGIC ../Magic)                         #将MAGIC变量指向本库地址即可
include(${MAGIC}/Library.cmake)
add_subdirectory(${MAGIC} Magic.out)
include_directories(
	${MAGIC}/Include			#Magic
	${MAGIC}/ThirdParty/Gzip		#Gzip
	${MAGIC}/ThirdParty/Asio/asio/include	#ASIO
	${MAGIC}/ThirdParty/RapidJSON/include	#RapidJSON
)
########################################################################################
```

## 联系方式
* QQ 614199451  
* QQ群 451405524
