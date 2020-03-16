<!--
 * @file: 
 * @Author: INotFound
 * @Date: 2020-03-16 00:33:44
 * @LastEditTime: 2020-03-16 01:18:18
 -->
# Magic服务端基础框架
&emsp;Magic 是一个能Web快速开发,且基于ASIO网络框架,拥有丰富的基础组件(Win And Linux)跨平台框架库。
## 快速示例
&emsp;本库将智能指针名称进行了重定义.
```
    /// 文件 Magic/Include/Magic/Core.h 中
    #define Safe std::unique_ptr
    #define Share std::shared_ptr
```
&emsp;在Main函数的第一行调用Magic的初始化方法,进行Config、Log相关组件的初始化工作.
```
    /// "Build_Test" 是项目名称.
    Magic::Init("Build_Test");
```
&emsp;Config配置组件的食用方法,目前只支持基础类型.
```
    /// 可自定义配置生成Json 和 ini格式的文件.具体常考 (Magic/Source/Magic.cpp) 文件。
    /// 未修改生成的配置文件的情况下,会使用at函数的第二个参数作为默认值.
    uint64_t number = Magic::Config::GetInstance()->at<uint64_t>("number", 24);
```
&emsp;Log日志组件的食用方法.
```
    /// 可自定义配置生成Html格式日志文件和普通日志文件.
    /// 可通过Magic::LogLevel枚举类调整日志输出等级.
    MAGIC_LOG(Magic::LogLevel::LogDebug) << "hello world";
```
&emsp;Plugin插件组件的食用方法.
```
    /// 详见以下两文件 
    -> Examples/Test_Plugin.cpp
    -> Examples/Test_PluginLib.cpp
```
&emsp;简单的Web服务器的食用方法.
```
    /// 详见以下文件 
    -> Examples/Test_Server.cpp
```
## 如何编译
### 编译器的支持
&emsp;只需支持C++11的编译器.
### 需要添加依赖的第三方库
&emsp;需初始化更新Git的子仓库(Asio、zlib、RapidJson).
```
    /// 在此Git仓库的根路径下调用以下命令。
    git submodule update --init
```
&emsp;加密库 OpenSSL ,数据库 MySql ,压缩库 zlib .
## 联系方式
&emsp;QQ 614199451  
&emsp;QQ群 451405524
