<!--
 * @Author: INotFound
 * @Date: 2021-01-18 15:17:40
 * @LastEditTime: 2022-02-10 11:25:34
-->
# Magic

## 简介

> ​		Use Standard C++ 11 && CMake.

> ​		基于IoC和AoP的快速开发库(C++ Rapid development library based on Inversion of Control And Aspect Oriented Programming Support),配合CMake构建套件开发,支持跨平台开发,并配备了更多的高性能组件.

## 特点

### 快速开发

---
- **高内聚,低耦合**.
- **无任何侵入式代码**.
- 超强的**通用性**以及高度**跨平台**.
- 实现**模块化**开发方式便于协作开发.
- 支持**Http/WebSocket**协议以及**WebServer**开发
- 基于 IoC`(Inversion of Control)` 以及 AoP`(Aspect Oriented Programming Support)`.

### 模块配置

---
- 支持灵活配置各个模块.
- 功能模块可高度**重用**，方便**扩展**以及**维护**.
- 使用**Json**格式配置文件`(*.json)`对功能进行模块划分.

### 安装

---

> 只需支持**C++11**的编译器以及**CMake**构建工具编译即可, 编译可选项: ***MySql*** ***Zlib*** ***SSL***.
- Linux
  1. **Gnu**构建套件 (跳转在库路径中)
    - `mkdir build && cd build && cmake ../. && make && make install`
  2. **Ninja**构建套件 (推荐.跳转在库路径中)
    - `mkdir build && cd build && cmake -GNinja ../. && ninja && ninja install`
- Win
  - VS 20XX
    - `cmake -G "Visual Studio 15 2017"` // or ***Visual Studio xx 20xx***

### 使用方法

---

1. 使用本库工具 Magic/Bin/***Create***
    1. 创建项目文件目录名,跳转项目文件目录中
    2. 使用 `Create Test /root/Magic` 创建项目模板, ***(Test 项目名)*** ***("/root/Magic" Magic库路径)***
    3. 当前目录会生成 ***CMakeLists.txt Main.cpp Test.json***三个文件
  
2. 打开***Test.json***配置文件,并添加上需要IoC管理的类.
4.  对***CMakeLists.txt***配置文件进行编辑添加**Test**模块(**Test.json**)
```cmake
set(MODULES
    ${MAGIC}/Modules/Magic.json
    ${PROJECT_SOURCE_DIR}/Test.json # Test.json 模块
)
```
5.  创建并跳转build目录中 调用 `cmake ../.` 并且进行Build.
6.  打开***Main.cpp***源文件,添加一下代码.
```c++
#include "Test.h" //添加头文件

int main(){
    auto application = std::make_shared<Magic::Application>();
    application->initialize([](const Safe<Magic::Container>& ioc){
    });
    return EXIT_SUCCESS;
}
```
5. ***Make Or Ninjia 编译即可***

### 本库示例

---

> 配置文件：[***Magic模块***](https://github.com/INotfound/Magic/blob/main/Modules/Magic.json) (可使用多个配置文件)
```jsonc
{                                                           // 日志以及配置模块示例
    "Configurations":{
        "Registered":[                                      // 类信息注册(强制,必须要具有一个).
            {
                "Id":"config",                              // 类Id标识(任意名).
                "Class":"Magic::Config",                    // 类名,如果有namespace,则需加上即可.
                "IncludePath": "Include/Core/Config.h",     // 类所在的文件路径(如何有相同的可以忽略).
                "Interface":"",                             // 继承的接口类,通常需要抽象的时候才使用.
                "Dependencies":[],                          // 依赖的其他类的类名.
                "FunctionPropertys":["addConfigFile"]       // 需要注册的属性函数.
            },
            {
                "Id":"configFile",                          // 与Magic::Config同文件,则不需要在此定义IncludePath
                "Class":"Magic::ConfigFile",
                "FunctionPropertys":["addFormatter"]
            },
            ......
        ],
        "Initialize":[                                      // 初始化(非强制)
            {
                "Id":"logger",
                "InvokeFunctions":{
                    "externMode":[]                         // 调用Id: logger 对象的externMode函数  
                }
            },
            {
                "Id":"logger",
                "Loop":true,                                // Loop 循环加入接口类对象
                "Callee":"Magic::ILogAppender",             // 接口类类型 若Loop == true则该属性必须具有值.
                "CalleeFunctions":["addILogAppender"],      // 接口类对象添加函数.
            }
        ]
    }
}
```
.....
## 注意
> 本库使用了异常,所有使用异常接口都会有对应 **warring** 注释标注.
```c++
try{
    ...
}catch(Magic::Failure&){
    ...
}
```
## 其他
  - [基础用法&演示地址一](https://www.bilibili.com/video/BV1V54y1x7KM)
  - [Web服务端&演示地址二](https://www.bilibili.com/video/BV1a5411H7af)
  - (未完待续).....

### 代码规范/联系方式
  - [C++](http://note.youdao.com/noteshare?id=0975fd51d320c1cd7bc0cbaab6d39e59&sub=AC10B1CBC6744F92B2B8A3F26DC47918)
- 作者联系方式 ***小白君 (INotfound)***

  - QQ: [614199451](http://wpa.qq.com/msgrd?v=3&uin=614199451&site=qq&menu=yes)
  - QQ群: [451405524](https://qm.qq.com/cgi-bin/qm/qr?k=qsjCo88_9j8cPCwkgzRzaIKfCyXU98VH&jump_from=webapi)