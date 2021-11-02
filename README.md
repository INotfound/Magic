<!--
 * @Author: INotFound
 * @Date: 2021-01-18 15:17:40
 * @LastEditTime: 2021-01-19 05:25:56
-->
# Magic

> Magic [v2.0.0] 正式发布.

## 简介

> ​		Use Standard C++ 11 && CMake.

> ​		基于IoC的快速开发库(C++ Rapid development library based on Inversion of Control),配合CMake构建套件开发,支持跨平台开发,并配备了更多的高性能组件.

## 特点

### 快速开发

- **高内聚,低耦合**.
- 超强的**通用性**以及高度**跨平台**.
- 实现**模块化**开发方式便于协作开发.
- 基于 IoC `(Inversion of Control)`的方式对每个类进行托管以及自动注入构建.

### 模块配置

- 功能模块可高度**重用**，方便**扩展**以及**维护**.
- 使用**Json**格式配置文件`(*.magic)`对功能进行模块划分.

### 非侵入式

- 高度解耦
- 代码**移植度**高.
- **无任何侵入式代码**.
### 安装

---

> 只需支持**C++11**的编译器以及**CMake**构建工具编译即可,  ***MySql、OpenSSl*** 编译可选项.
- Linux
  1. **Gnu**构建套件 (跳转在库路径中)
    - `mkdir build && cd build && cmake ../. && make && make install`
  2. **Ninja**构建套件 (推荐.跳转在库路径中)
    - `mkdir build && cd build && cmake -GNinja ../. && ninja && ninja install`
- Win
  - VS 20XX
    - `cmake -G "Visual Studio 15 2017"` // or ***Visual Studio xx 20xx***
> `*.magic` 模块每次被修改完成时,请调用一次 `cmake ../. `

### 使用方法

---

1. 使用本库工具 Magic/Bin/***Create***
  1. 创建项目文件目录名,跳转项目文件目录中
  2. 使用 `Create Test /root/Magic` 创建项目模板, ***(Test 项目名)*** ***("/root/Magic" Magic库路径)***
  3. 当前目录会生成 ***CMakeLists.txt Main.cpp Test.magic***三个文件

2. 查看***Test***项目文件夹中***CMakeLists.txt***文件.
```cmake
#Please Add The Directory Path Of The Magic Library.
set(MAGIC /root/Magic)

if(NOT DEFINED MAGIC)
    message(FATAL_ERROR "Please Add The Directory Path Of The Magic Library!!!")
endif()

include_directories(
    ${MAGIC}
	${MAGIC}/Include
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/Include
    ${MAGIC}/ThirdParty/Asio/Include
    ${MAGIC}/ThirdParty/RapidJSON/Include
)
link_directories(
    ${MAGIC}/Lib
)
link_libraries(Magic)
include(${MAGIC}/Magic.cmake)

#参数[1]:本库路径
#参数[2]:导出的头文件名
#参数[MODULE]: 单模块(Module) or 多模块(Modules)
GENEX(${MAGIC} Test MODULE
    ${MAGIC}/Magic.magic
)
```
3. 打开***Test.magic***配置文件,并添加上自定义的类.
```jsonc
{
    "Configurations":{
        "NameSpace":"Test",                 // 同 C++ 的namespace
        "Registered":[                      // 类信息注册(强制,必须要具有一个).
            {
                "Id":"",                    // 类Id标识(任意名).
                "Class":"",                 // 类名,如果有namespace,则需加上即可(XXX::ClassName).
                "Interface":"",             // 继承的接口类,通常需要抽象的时候才使用.
                "IncludePath": "",          // 类所在的文件路径(如何有相同的可以忽略).
                "Dependencies":[],          // 依赖的其他类的类名.
                "FunctionPropertys":[]      // 需要注册的属性函数.
            }
        ],
        "Initialize":[                      // 初始化(非强制)
            {
                "Id":"",                    // 类Id标识应与上方Registered中一致.
                "Loop":false,               // Loop 循环初始化
                "Callee":"",                // 接口类类型 若Loop == true则该属性必须具有值.
                "FunctionPropertys":[],     // 需要初始化的函数.
                "FunctionArguments":{}      // 函数中对应的 RAW Arguments. “XX”:["XXX"]写法.
            }
        ],
        "Constructor":{                     // 构造函数定义
            "Name":"Initialize",            // 暴露给main函数中调用名.
            "WithParameter": false          // 是否需要自定义注册参数.
        }
    }
}
```
4.  对***CMake***配置文件在**SRC**后方进行编辑添加**Test**模块(**Test.magic**)
```cmake
GENEX(${MAGIC} Test MODULE
    ${MAGIC}/Magic.magic
    ${PROJECT_SOURCE_DIR}/Test.magic
)
```
5.  跳转build目录中 调用 `cmake ../.` or `cmake ../. -Gxxx`
  1.  打开***Main.cpp***源文件
```c++
#include "Test.h" //添加头文件

int main(){
    Test::Initialize(); //添加初始化代码. [Configurations.NameSpace]::[Constructor.Name]命名设定
    return EXIT_SUCCESS;
}
```
5. ***编译即可***
### 示例

---

> 目录位置:[Magic/Examples](https://github.com/INotfound/Magic/tree/main/Examples)

#### [***Base***](https://github.com/INotfound/Magic/tree/main/Examples/Base)示例

> CMakeLists.txt 引用Magic模块

```cmake
GENEX(${MAGIC} Base MODULE
	${MAGIC}/Magic.magic
)
```
```c++
#include "Base.h"

int main(){
    Base::Initialize();
    MAGIC_DEBUG() << "hello world";
    return EXIT_SUCCESS;
}
```
#### [***WebServer***](https://github.com/INotfound/Magic/tree/main/Examples/WebServer)示例
> ***Main.cpp***
```c++
#include "Web.h"

int main(){
    Web::Initialize();
    return EXIT_SUCCESS;
}
```
> ***Servlet.magic***
```json
{
    "Configurations":{
        "NameSpace":"Web",
        "Registered":[
            {
                "Id":"testSevlet",
                "Class":"Web::TestServlet",
                "IncludePath": "TestSevlet.h",
                "Interface":"Magic::NetWork::Http::IHttpServlet",
                "Dependencies":[],
                "FunctionPropertys":[]
            }
        ],
        "Initialize":[],
        "Constructor":{
            "Name":"Initialize",
            "WithParameter": false
        }
    }
}
```
> ***TestSevlet.h***
```c++
#pragma once
#include <Magic>

namespace Web{
    class TestServlet :public Magic::NetWork::Http::IHttpServlet{
    public:
        TestServlet();
        bool handle(const Safe<Magic::NetWork::Http::HttpRequest>& request,const Safe<Magic::NetWork::Http::HttpResponse>& response) override;
    };
}
```
> ***TestSevlet.cpp***
```c++
#include "TestSevlet.h"

namespace Web{
    TestServlet::TestServlet()
        :Magic::NetWork::Http::IHttpServlet("",Magic::NetWork::Http::HttpServletType::Deafult){
    }
    bool TestServlet::handle(const Safe<Magic::NetWork::Http::HttpRequest>& request,const Safe<Magic::NetWork::Http::HttpResponse>& response){
        response->setStatus(Magic::NetWork::Http::HttpStatus::NOT_FOUND);
        std::string notfound{R"Template(<html>
            <head><title>404 Not Found</title></head>
            <body>
            <center><h1>404 Not Found</h1></center>
            <hr><center>Magic/0.0.2</center>
            </body>
            </html>)Template"};
        response->setBody(notfound);
        return true;
    }
}
```
> ***CMakeLists.txt***
```cmake
GENEX(${MAGIC} Web MODULE
    ${MAGIC}/Magic.magic
    ${MAGIC}/WebServer.magic
    ${PROJECT_SOURCE_DIR}/Servlet.magic
)
```
### 本库示例

---

> 配置文件：[***Magic模块***](https://github.com/INotfound/Magic/blob/master/Magic.magic)(可使用多个配置文件)
```jsonc
{                                                           // 日志以及配置模块示例
    "Configurations":{
        "NameSpace":"Magic",                                // 同C++的namespace.
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
                "Id":"configFile",                          // 类Id标识应与上方Registered中一致.
                "FunctionPropertys":["setFilePath"],        // 需要初始化的函数.
                "FunctionArguments":{
                    "setFilePath" : ["\"./config.conf\""]   // 函数中对应的 RAW Arguments. “XX”:["XXX"]写法.
                }
            },
            {
                "Id":"logger",
                "Loop":true,                                // Loop 循环加入接口类对象
                "Callee":"Magic::ILogAppender",             // 接口类类型 若Loop == true则该属性必须具有值.
                "FunctionPropertys":["addILogAppender"],    // 接口类对象添加函数.	
                "FunctionArguments":{}
            }
        ],
        "Constructor":{                                     // 构造函数定义
            "Name":"Initialize",                            // 暴露给main函数中调用名.
            "WithParameter": false                          // 是否需要自定义注册参数.
        }
    }
}
```
.....
## 其他
  - [基础用法&演示地址一](https://www.bilibili.com/video/BV1V54y1x7KM)
  - [Web服务端&演示地址二](https://www.bilibili.com/video/BV1a5411H7af)
  - (未完待续).....

### 代码规范/联系方式
  - [C++](http://note.youdao.com/noteshare?id=0975fd51d320c1cd7bc0cbaab6d39e59&sub=AC10B1CBC6744F92B2B8A3F26DC47918)
- 作者联系方式 ***小白君 (INotfound)***

  - QQ: [614199451](http://wpa.qq.com/msgrd?v=3&uin=614199451&site=qq&menu=yes)
  - QQ群: [451405524](https://qm.qq.com/cgi-bin/qm/qr?k=qsjCo88_9j8cPCwkgzRzaIKfCyXU98VH&jump_from=webapi)