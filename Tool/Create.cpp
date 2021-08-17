/*
 * @Author: INotFound
 * @Date: 2020-12-21 23:33:29
 * @LastEditTime: 2021-01-25 16:41:57
 */
#include <fstream>
#include <iostream>

#if defined(_WIN32) || defined(_WIN64)
    #include <io.h>
    #include <windows.h>
    #define IS_FILE(Path)        _access(Path,0)
#endif

#if defined(linux) || defined(__linux__)
    #include <unistd.h>
    #define IS_FILE(Path)        access(Path,0)
#endif

std::string g_LF = "\n";
std::string g_MagicPath;
std::string g_MainCpp=R"Template(#include <Magic/Magic>

int main(int argc, char** argv){
    return EXIT_SUCCESS;
})Template";
std::string g_Module = R"Template({
    "Configurations":{
        "NameSpace":"",
        "Registered":[
            {
                "Id":"",
                "Class":"",
                "Interface":"",
                "IncludePath": "",
                "Dependencies":[],
                "FunctionPropertys":[]
            }
        ],
        "Initialize":[
            {
                "Id":"",
                "Loop":false,
                "Callee":"",
                "FunctionPropertys":[],
                "FunctionArguments":{}
            }
        ],
        "Constructor":{
            "Name":"Initialize",
            "WithParameter": false
        }
    }
})Template";

std::string g_Template = R"Template({
    "Configurations":{
        "NameSpace":"",                     // 同C++的namespace,若不想具有namespace留空即可.
        "Registered":[                      // 类信息注册(强制,必须要具有一个).
            {
                "Id":"",                    // 类Id标识(任意名)用于Initialize中使用.
                "Class":"",                 // 类名,如果有namespace,则需加上即可.
                "Interface":"",             // 类所在的文件路径(如何有相同的可以忽略).
                "IncludePath": "",          // 继承的接口类,通常需要抽象的时候才使用.
                "Dependencies":[],          // 依赖的其他类的类名.
                "FunctionPropertys":[]      // 需要注册的属性函数.
            }
        ],
        "Initialize":[                      // 初始化(非强制)
            {
                "Id":"",                    // 类Id标识应与上方Registered中一致.
                "Loop":false,               // Loop 循环加入接口类对象
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
})Template";

void OutPutMainCpp(){
    std::ofstream ostream;
    ostream.open("Main.cpp",std::ios::out);
    if(!ostream.is_open()){
        std::printf("Main.cpp output failed.\n");
        return;
    }
    ostream << g_MainCpp;
    ostream.flush();
    ostream.close();
}

void OutPutModuleFile(const std::string& name){
    std::ofstream ostream;
    std::string moduleFile = name + ".magic";
    ostream.open(moduleFile,std::ios::out);
    if(!ostream.is_open()){
        std::printf("%s output failed.\n",moduleFile.c_str());
        return;
    }
    ostream << g_Module;
    ostream.flush();
    ostream.close();
}
void OutPutGuideFile(){
    std::ofstream ostream;
    std::string guideFile = "Guide.magic";
    ostream.open(guideFile,std::ios::out);
    if(!ostream.is_open()){
        std::printf("%s output failed.\n",guideFile.c_str());
        return;
    }
    ostream << g_Template;
    ostream.flush();
    ostream.close();
}

void OutPutCMakeFile(const std::string& name){
    std::ofstream ostream;
    ostream.open("CMakeLists.txt",std::ios::out);
    if(!ostream.is_open()){
        std::printf("CMakeFile output failed.\n");
        return;
    }
    ostream << "cmake_minimum_required(VERSION 3.0)" << g_LF << g_LF
            << "project(" << name << ")" << g_LF
            << "set(CMAKE_CXX_STANDARD 11)" << g_LF
            << "set(CMAKE_EXPORT_COMPILE_COMMANDS ON)" << g_LF << g_LF
            << "#Please Add The Directory Path Of The Magic Library." << g_LF
            << "set(MAGIC " << g_MagicPath << ")" << g_LF << g_LF
            << "if(NOT DEFINED MAGIC)" << g_LF
            << "    message(FATAL_ERROR \"Please Add The Directory Path Of The Magic Library!!!\")" << g_LF
            << "endif()" << g_LF << g_LF
            << "include_directories(" << g_LF
            << "    ${MAGIC}" << g_LF
            << "    ${MAGIC}/Include" << g_LF
            << "    ${PROJECT_SOURCE_DIR}" << g_LF
            << "    ${MAGIC}/ThirdParty/Asio/Include" << g_LF
            << "    ${MAGIC}/ThirdParty/RapidJSON/Include" << g_LF
            << ")" << g_LF
            << "link_directories(" << g_LF
            << "    ${MAGIC}/Lib" << g_LF
            << ")" << g_LF
            << "link_libraries(Magic)" << g_LF 
            << "include(${MAGIC}/Magic.cmake)" << g_LF 
            << "include_directories(${CMAKE_BINARY_DIR})" << g_LF << g_LF
            << "set(MODULES" << g_LF
            << "    ${MAGIC}/Modules/Magic.magic" << g_LF
            << ")" << g_LF << g_LF
            << "add_custom_target(Gen ALL)" << g_LF
            << "add_custom_command(" << g_LF
            << "    TARGET Gen" << g_LF
            << "    COMMAND ${MAGIC}/Bin/Gen ${MODULES} " << name << g_LF
            << ")" << g_LF << g_LF
            << "add_executable(" << name <<" Main.cpp)";
    ostream.flush();
    ostream.close();
}


int main(int argc, char** argv){
    if(argc < 3){
        std::printf("Plase Input Project Name And Magic Library Path.\n");
        std::printf("\tParam: [1]: Project Name [2]: Magic Library Path.\n");
        return EXIT_FAILURE;
    }

    g_MagicPath = argv[2];
    std::string path = g_MagicPath;
    path += "/Magic.cmake";
    if(IS_FILE(path.c_str()) != 0){
        std::printf("Magic Library Error Path!\n");
        return EXIT_FAILURE;
    }
    OutPutMainCpp();
    OutPutCMakeFile(argv[1]);
    OutPutModuleFile(argv[1]);
    return EXIT_SUCCESS;
}
