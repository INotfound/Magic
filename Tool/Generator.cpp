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
    std::vector<std::string> CalleeFunctions;
    std::vector<std::pair<std::string,std::vector<std::string>>> InvokeFunctions;
}Initialize;

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
        if(!v.HasMember("Id")){
            std::printf("[Err]: Initialize Json[Id] Missing Fields:%d !!!\n",__LINE__);
            std::exit(1);
        }
        initializeObj.Id = v["Id"].GetString();
        if(v.HasMember("CalleeFunctions") && v["CalleeFunctions"].IsArray()){
            auto functionPropertys = v["CalleeFunctions"].GetArray();
            for(auto& fps : functionPropertys){
                std::string fpsString = fps.GetString();
                initializeObj.CalleeFunctions.push_back(fpsString);
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

        if(v.HasMember("InvokeFunctions") && v["InvokeFunctions"].IsObject()){
            auto InvokeFunctions = v["InvokeFunctions"].GetObject();
            for(auto& fas : InvokeFunctions){
                if(!fas.value.IsArray()){
                    std::printf("[Err]: Initialize Json[InvokeFunctions] Missing Fields:%d !!!\n",__LINE__);
                    std::exit(1);
                }

                std::vector<std::string> arguments;
                auto argumentsArray = fas.value.GetArray();

                if(argumentsArray.Empty()){
                    initializeObj.InvokeFunctions.emplace_back(fas.name.GetString(),arguments);
                }else{
                    for(auto& multi : argumentsArray){
                        if(multi.IsArray()){
                            auto argumentArray = multi.GetArray();
                            for(auto& arg : argumentArray){
                                std::string argString = arg.GetString();
                                arguments.push_back(argString);
                            }
                            initializeObj.InvokeFunctions.emplace_back(fas.name.GetString(),arguments);
                            arguments.clear();
                        }else{
                            std::string argString = multi.GetString();
                            arguments.push_back(argString);
                        }
                    }
                    if(!arguments.empty()){
                        initializeObj.InvokeFunctions.emplace_back(fas.name.GetString(),arguments);
                    }
                }
            }
        }

        if(initializeObj.Id.empty() && (initializeObj.InvokeFunctions.empty() || initializeObj.CalleeFunctions.empty())){
            std::printf("[Err]: Initialize Json[Id && (InvokeFunctions || CalleeFunctions)] Is Empty:%d !!!\n",__LINE__);
            std::exit(1);
        }
        g_InitializeMap.push_back(initializeObj);
    }
}

void Generator(std::ostream& stream){
    std::string LF("\n");
    stream << "/*" << LF
           << " * @Author: INotFound" << LF
           << " * @Link: https://github.com/INotfound/Magic" << LF
           << " */" << LF;
    {
        stream << "#pragma once" << LF
               << "#include <Magic/Magic>" << LF;
        for(auto v : g_RegisteredMap){
            if(!v.IncludePath.empty()){
                stream << "#include \""
                    << v.IncludePath
                    << "\"" << LF;
            }
        }
        stream << LF;
    }


    stream << "namespace Magic{"
           << LF;


    stream << "    const Safe<Magic::Container>& Application::initialize"
           << "(const std::function<void(const Safe<Magic::Container>&)>& configure){";
    stream << LF;

    {   /// Registereds
        for(auto val : g_RegisteredMap){
            if(val.Interface.empty()){
                stream << "        m_Container->registerType<";
            }else{
                stream << "        m_Container->registerTypeEx<"
                    << "::"
                    << val.Interface 
                    << ",";
            }
            stream << "::" << val.Class;

            for(auto& deps : val.Dependencies){
                if(!deps.empty()){
                    stream << ","
                           << "Safe<"
                           << "::"
                           << deps
                           << ">";
                }
            }
            stream << ">()";
            for(auto& fps : val.FunctionPropertys){
                stream << ".registerProperty(&";
                stream << "::" << val.Class << "::";
                stream << fps
                       << ")";
            }
            stream << ";" << LF;
        }
    }
    
    { /// Constructor
        stream << "        if(configure)" << LF
               << "            configure(m_Container);" << LF;
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
                       << "m_Container->resolve<";
                if(CallerIter.Interface.empty()){
                    stream << "::" << CallerIter.Class;
                }else{
                    stream << "::" << CallerIter.Interface << ','  <<  "::" << CallerIter.Class;
                }
                stream << ">();"
                       << LF
                       << "            for(auto& v : "
                       << "m_Container->resolveAll<"
                       << val.Callee
                       << ">()){"
                       << LF
                       << "                "
                       << val.Id
                       << "->"
                       << val.CalleeFunctions.at(0)
                       << "(v);"
                       << LF
                       << "            }"
                       << LF;
            }else{
                stream << "            auto "
                       << val.Id << " = "
                       << "m_Container->resolve<";
                if(CallerIter.Interface.empty()){
                    stream << "::" << CallerIter.Class;
                }else{
                    stream << "::" << CallerIter.Interface << ','  << "::" << CallerIter.Class;
                }
                stream << ">();"
                       << LF;
                /// todo: 可以扩展一下
                for(auto& ifs : val.InvokeFunctions){
                    stream << "            "
                        << val.Id
                        << "->"
                        << ifs.first
                        << "(";
                    for(uint32_t i = 0;i < ifs.second.size();i++){
                        if(i != 0){
                            stream << ",";
                        }
                        const std::string& fas = ifs.second.at(i);
                        iter = g_RegisteredMap.begin();
                        for(;iter != g_RegisteredMap.end();iter++){
                            if(fas == iter->Id){
                                break;
                            }
                        }
                        if(iter == g_RegisteredMap.end()){
                            stream << fas;
                        }else{
                            stream << "m_Container->resolve<" << "::" << iter->Class << ">()";
                        }
                    }
                    stream << ");" << LF;
                }
            }
            stream << "        }" << LF;
        }  
    }
    stream << "        "
           << "m_Container->resolveAll();"
           << LF;
    {
        stream << "        return m_Container;"
            << LF
            << "    }"
            << LF
            << "}";
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

    auto object = jsonDoc["Configurations"].GetObject();

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


    std::stringstream bufferStream;
    Generator(bufferStream);
    std::string gen = bufferStream.str();

    std::ifstream inStream;
    inStream.open(headerFile,std::ios::in);
    if(inStream.is_open()){
        std::stringstream fileTextStream;
        fileTextStream << inStream.rdbuf();
        std::string file = fileTextStream.str();
        if(gen == file){
            return;
        }
    }
    inStream.close();

    std::ofstream outStream;
    outStream.open(headerFile,std::ios::out);
    if(!outStream.is_open()){
        std::printf("[Err]: File Generation Failed: %s\n",path.c_str());
        std::exit(1);
    }
    outStream << gen;
    outStream.flush();
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
