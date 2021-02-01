/*
 * @Author: INotFound
 * @Date: 2020-12-17 00:06:57
 * @LastEditTime: 2021-02-01 23:30:06
 */
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include "RapidJson/document.h"
#include "RapidJson/rapidjson.h"

typedef struct{
    std::string Id;
    std::string Class;
    std::string Interface;
    std::string IncludePath;
    std::vector<std::string> Dependencies;
    std::vector<std::string> FunctionPropertys;
}Registered;

typedef struct{
    bool Loop;
    std::string Id;
    std::string Callee;
    std::vector<std::string> FunctionPropertys;
    std::unordered_map<std::string,std::vector<std::string>> FunctionArguments;
}Initialize;

std::string g_NameSpace;
std::string g_HeaderName;
std::string g_Constructor;
bool g_ConstructorWithParameter;
std::vector<Initialize> g_InitializeMap;
std::vector<Registered> g_RegisteredMap;



void ParseRegistered(const rapidjson::Value::Array& array){
    for(auto& v : array){
        Registered registerObj;
        if(!v.HasMember("Id") || !v.HasMember("Class")){
            std::printf("[Err]: Registered Json[Id|Class] Missing Fields:%d !!!\n",__LINE__);
            std::exit(1);
        }
        registerObj.Id = v["Id"].GetString();
        registerObj.Class = v["Class"].GetString();
        if(v.HasMember("IncludePath")){
            registerObj.IncludePath = v["IncludePath"].GetString();
        }
        
        if(v.HasMember("Interface")){
            registerObj.Interface = v["Interface"].GetString();
        }

        if(v.HasMember("Dependencies") && v["Dependencies"].IsArray()){
            auto dependencies = v["Dependencies"].GetArray();
            for(auto& deps : dependencies){
                std::string depsString = deps.GetString();
                registerObj.Dependencies.push_back(depsString);
            }
        }

        if(v.HasMember("FunctionPropertys") && v["FunctionPropertys"].IsArray()){
            auto functionPropertys = v["FunctionPropertys"].GetArray();
            for(auto& fps : functionPropertys){
                std::string fpsString = fps.GetString();
                registerObj.FunctionPropertys.push_back(fpsString);
            }
        }
        
        if(registerObj.Id.empty() || registerObj.Class.empty()){
            std::printf("[Err]: Registered Json[Id|Class] Is Empty:%d !!!\n",__LINE__);
            std::exit(1);
        }
        g_RegisteredMap.emplace_back(registerObj);
    }
}

void ParseInitialize(const rapidjson::Value::Array& array){
    for(auto& v : array){
        Initialize initializeObj;
        initializeObj.Loop = false;
        if(!v.HasMember("Id") || !v.HasMember("FunctionPropertys")){
            std::printf("[Err]: Initialize Json[Id|FunctionPropertys] Missing Fields:%d !!!\n",__LINE__);
            std::exit(1);
        }
        initializeObj.Id = v["Id"].GetString();
        if(v.HasMember("FunctionPropertys") && v["FunctionPropertys"].IsArray()){
            auto functionPropertys = v["FunctionPropertys"].GetArray();
            for(auto& fps : functionPropertys){
                std::string fpsString = fps.GetString();
                initializeObj.FunctionPropertys.push_back(fpsString);
            }
        }

        if(v.HasMember("Loop") && v["Loop"].IsBool()){
            initializeObj.Loop = v["Loop"].GetBool();
            if(initializeObj.Loop){
                if(!v.HasMember("Callee")){
                    std::printf("[Err]: Initialize Json[Callee] Missing Fields:%d !!!\n",__LINE__);
                    std::exit(1);
                }
                initializeObj.Callee = v["Callee"].GetString();
            }
        }

        if(v.HasMember("FunctionArguments") && v["FunctionArguments"].IsObject()){
            auto functionArguments = v["FunctionArguments"].GetObject();
            for(auto& fas : functionArguments){
                if(!fas.value.IsArray()){
                    std::printf("[Err]: Initialize Json[FunctionArguments] Missing Fields:%d !!!\n",__LINE__);
                    std::exit(1);
                }
                std::vector<std::string> arguments;
                auto argumentArray = fas.value.GetArray();
                for(auto& arg : argumentArray){
                    std::string argString = arg.GetString();
                    arguments.push_back(argString);
                }
                initializeObj.FunctionArguments.emplace(fas.name.GetString(),arguments);
            }

        }
        if(initializeObj.Id.empty() || initializeObj.FunctionPropertys.empty()){
            std::printf("[Err]: Initialize Json[Id|FunctionPropertys] Is Empty:%d !!!\n",__LINE__);
            std::exit(1);
        }
        g_InitializeMap.push_back(initializeObj);
    }
}

void Generator(std::ofstream& stream){
    std::string LF("\n");
    stream << "/*" << LF
           << " * @Author: INotFound" << LF
           << " * @Link: https://github.com/INotfound/Magic" << LF
           << " */" << LF;
    {
        stream << "#pragma once" << LF
               << "#include <Core/Container.h>" << LF;
        for(auto v : g_RegisteredMap){
            if(!v.IncludePath.empty()){
                stream << "#include \""
                    << v.IncludePath
                    << "\"" << LF;
            }
        }
        stream << LF;
    }

    if(!g_NameSpace.empty()){
        stream << "namespace "
               << g_NameSpace
               << "{"
               << LF;
    }
    
    stream << "    const Safe<Magic::Container>& "
           << g_Constructor;
    if(g_ConstructorWithParameter){
        stream << "(const std::function<void(const Safe<Magic::Container>&)>& configure){";
    }else{
        stream << "(){";
    }
    stream << LF;

    {   /// Registereds
        stream << "        const auto& iocContainer = Magic::Configure([](const Safe<Magic::Container>& ioc){"
               << LF;
 
        for(auto val : g_RegisteredMap){
            if(val.Interface.empty()){
                stream << "            ioc->registerType<";
            }else{
                stream << "            ioc->registerTypeEx<"
                    << val.Interface 
                    << ",";
            }
            stream << val.Class;

            for(auto& deps : val.Dependencies){
                if(!deps.empty()){
                    stream << ","
                           << deps;
                }
            }
            stream << ">()";
            for(auto& fps : val.FunctionPropertys){
                stream << ".registerProperty(&";
                stream << val.Class << "::";
                stream << fps
                       << ")";
            }
            stream << ";" << LF;
        }
        stream << "        });" << LF;
    }
    
    { /// Constructor
        if(g_ConstructorWithParameter){
            stream << "        configure(iocContainer);" << LF;
        }
    }

    { /// Initializes
        for(auto& val : g_InitializeMap){
            stream << "        {" << LF;
            auto iter = g_RegisteredMap.begin();
            for(;iter != g_RegisteredMap.end();iter++){
                if(iter->Id == val.Id){
                    break;
                }
            }
            if(iter == g_RegisteredMap.end()){
                std::printf("[Err]: Json[Caller] Is Empty:%d !!!\n",__LINE__);
                std::exit(1);
            }
            auto CallerIter = *iter;
            if(val.Loop){
                stream << "            "
                       << "auto "
                       << val.Id << " = "
                       << "iocContainer->resolve<"
                       << CallerIter.Class
                       << ">();"
                       << LF
                       << "            for(auto& v : "
                       << "iocContainer->resolveAll<"
                       << val.Callee
                       << ">()){"
                       << LF
                       << "                "
                       << val.Id
                       << "->"
                       << val.FunctionPropertys.at(0)
                       << "(v);"
                       << LF
                       << "            }"
                       << LF;
            }else{
                stream << "            auto "
                       << val.Id << " = "
                       << "iocContainer->resolve<"
                       << CallerIter.Class
                       << ">();"
                       << LF;
                /// todo: 可以扩展一下
                for(auto& fps : val.FunctionPropertys){
                    stream << "            "
                        << val.Id
                        << "->"
                        << fps
                        << "(";

                    uint32_t i = 0;
                    for(auto& fas : val.FunctionArguments[fps]){
                        if(i == 0){
                            i++;
                            stream << fas;
                            continue;
                        }
                        stream << "," << fas;
                    }
                    stream << ");" << LF;
                }
            }
            stream << "        }" << LF;
        }  
    }
    stream << "        "
           << "iocContainer->resolveAll();"
           << LF;
    {
        stream << "        return iocContainer;"
            << LF
            << "    }"
            << LF;
        if(!g_NameSpace.empty()){
            stream << "}";
        }
    }
    std::printf("Code Generation Successful!!!\n");
}

bool CheckConfiguration(const std::string& path){
std::ifstream istream;
    istream.open(path,std::ios::in);
    if(!istream.is_open()){
        std::printf("[Err]: File Path: %s\n",path.c_str());
        return false;
    }

    std::ostringstream jsonContent;
    jsonContent << istream.rdbuf();
    rapidjson::Document jsonDoc;
    
    if(jsonDoc.Parse<rapidjson::ParseFlag::kParseCommentsFlag>(jsonContent.str().c_str()).HasParseError()){
        std::printf("[Err]: Invalid JsonDocument!!!\n");
        return false;
    }

    if(!jsonDoc.HasMember("Configurations")){
        std::printf("[Err]: Wrong JsonDocument Format Code!!\n");
        return false;
    }
    g_Constructor = "Configure";
    g_ConstructorWithParameter = false;

    auto object = jsonDoc["Configurations"].GetObject();

    if(object.HasMember("NameSpace")){
        std::string nameSpace = object["NameSpace"].GetString();
        if(!nameSpace.empty()){
            g_NameSpace = nameSpace;
        }
    }

    if(object.HasMember("Constructor")){
        auto constructor = object["Constructor"].GetObject();
        if(constructor.HasMember("Name")){
            g_Constructor = constructor["Name"].GetString();
        }
        if(constructor.HasMember("WithParameter")){
            g_ConstructorWithParameter = constructor["WithParameter"].GetBool();
        }
    }

    if(!object.HasMember("Registered")){
        std::printf("[Err]: Unused Registered!!!\n");
        return false;
    }

    if(!object["Registered"].IsArray()){
        std::printf("[Err]: Registered Is Not An Array Type!!!\n");
        return false;
    }
    ParseRegistered(object["Registered"].GetArray());
    
    if(object.HasMember("Initialize")){
        ParseInitialize(object["Initialize"].GetArray());
    }else{
        std::printf("[Warn]: Unused Initialize !!!\n");
    }
    return true;
}

void OutPutCppFile(const std::string& path){
    std::string headerFile(path);
    headerFile += ".h";
    std::ofstream ostream;
    ostream.open(headerFile,std::ios::out);
    if(!ostream.is_open()){
        std::printf("[Err]: File Generation Failed: %s\n",path.c_str());
        std::exit(1);
    }
    Generator(ostream);
}

int main(int argc, char** argv){
    if(argc < 3){
        std::printf("Invalid Argument!!!\n");
        return EXIT_FAILURE;
    }
    for(auto i = 1; i< (argc - 1);i++){
        if(!CheckConfiguration(argv[i])){
            return EXIT_FAILURE;
        }
    }
    OutPutCppFile(argv[argc-1]);
    return EXIT_SUCCESS;
}