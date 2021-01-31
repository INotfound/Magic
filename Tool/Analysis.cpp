#include <list>
#include <string>
#include <fstream>
#include <iostream>
#include <unordered_map>
#include <clang-c/Index.h>

struct FunctionDecl {
    bool m_IsConstructor;
    std::string m_ReturnType;
    std::string m_FunctionName;
    std::list<std::string> m_ParamType;
};

struct ClassDecl {
    std::string m_ClassName;
    std::string m_InterfaceName;
    std::list<FunctionDecl> m_MemberMethods;
};

struct NamespaceDecl {
    std::string m_NamespaceName;
    std::unordered_map<std::string,ClassDecl> m_Classes;
};

struct ClassContainer{
    ClassDecl m_Classs;
    std::string m_FilePath;
};

void VisitNamespace(CXCursor cursor);
void VisitClass(CXCursor cursor);
void VisitMemberFunction(CXCursor cursor);
void VisitParam(CXCursor cursor);

static bool isMark = false;
static bool g_Debug = false;
static std::string g_FilePath;
static std::string g_NameSpace;
static class Visitor* g_Visitor;
static std::string g_FunctionName;
static bool g_WithParameter = false;
static uint32_t g_IncludeCount = 1;
static uint32_t g_HeaderFileCount = 0;
static const char *g_Args[256] = {"-xc++"};
static const char *g_HeaderFiles[256] = {0};

class Visitor {
    friend void VisitNamespace(CXCursor cursor);
    friend void VisitClass(CXCursor cursor);
    friend void VisitMemberFunction(CXCursor cursor);
    friend void VisitParam(CXCursor cursor);
public:
    Visitor(const char* arg[],uint32_t count,std::string fileName,bool debug){
        m_Index = clang_createIndex(0, debug);
        m_Unit = clang_parseTranslationUnit(
            m_Index, fileName.c_str(), 
            arg, count, nullptr, 0, 
            CXTranslationUnit_SkipFunctionBodies | CXTranslationUnit_LimitSkipFunctionBodiesToPreamble
        );
        if (m_Unit == nullptr)
            std::cout << "TranslationUnit parse failed" << std::endl;
    }

    ~Visitor(){
        clang_disposeTranslationUnit(m_Unit);
        clang_disposeIndex(m_Index);
    }

    void visit(std::string namespaceSpecifier){
        m_VisitNamespace = namespaceSpecifier;
        g_Visitor = this;
        if (m_Unit != nullptr){
            CXCursor cursor = clang_getTranslationUnitCursor(m_Unit);
            clang_visitChildren(
                cursor,
                [](CXCursor cursor, CXCursor parent,CXClientData client_data)->CXChildVisitResult{
                    CXCursorKind kind = clang_getCursorKind(cursor);
                    if (kind == CXCursor_Namespace && std::string(clang_getCString(clang_getCursorSpelling(cursor))) == g_Visitor->m_VisitNamespace){
                        VisitNamespace(cursor);
                    }
                    return CXChildVisit_Continue;
                }, 
                nullptr
            );
        }
    }
    std::list<NamespaceDecl> &getAst() { return m_NamespaceDecl; };

private:
    CXIndex m_Index;
    CXTranslationUnit m_Unit;
    std::string m_VisitNamespace;
    ClassDecl m_ConstructingClass;
    FunctionDecl m_ConstructingFunction;
    NamespaceDecl m_ConstructingNamespace;
    std::list<NamespaceDecl> m_NamespaceDecl;
};

void VisitNamespace(CXCursor cursor){
    clang_visitChildren(
        cursor, 
        [](CXCursor cursor, CXCursor parent, CXClientData client_data)->CXChildVisitResult
        {
            CXCursorKind kind = clang_getCursorKind(cursor);
            // assume only have class in namespace
            if (kind == CXCursor_ClassDecl)
                VisitClass(cursor);
            return CXChildVisit_Continue;
        } , nullptr);
    g_Visitor->m_ConstructingNamespace.m_NamespaceName = clang_getCString(clang_getCursorSpelling(cursor));
    g_Visitor->m_NamespaceDecl.emplace_back(g_Visitor->m_ConstructingNamespace);
    g_Visitor->m_ConstructingNamespace = NamespaceDecl();
}

void VisitClass(::CXCursor cursor){
    clang_visitChildren(
        cursor, 
        [](CXCursor cursor,CXCursor parent,CXClientData client_data)->CXChildVisitResult{
            CXCursorKind kind = clang_getCursorKind(cursor);
            // assume only have methods in class
            if (kind == CXCursor_CXXMethod || kind == CXCursor_Constructor){
                VisitMemberFunction(cursor);
                CXCursor *overridden;
                uint32_t numOverridden = 0;
                clang_getOverriddenCursors(cursor, &overridden, &numOverridden);
                for (uint32_t i = 0; i < numOverridden; ++i) {
                    CXCursor super = clang_getCursorLexicalParent(overridden[i]);
                    std::string superName = clang_getCString(clang_getCursorSpelling(clang_getCursorLexicalParent(super)));
                    if(superName.find(".h") == std::string::npos){
                        g_Visitor->m_ConstructingClass.m_InterfaceName.assign(superName);
                        g_Visitor->m_ConstructingClass.m_InterfaceName.append("::");
                    }
                    g_Visitor->m_ConstructingClass.m_InterfaceName.append(clang_getCString(clang_getCursorSpelling(super)));
                }
                clang_disposeOverriddenCursors(overridden);
            }
            return CXChildVisit_Continue;
        } , nullptr);
    g_Visitor->m_ConstructingClass.m_ClassName = clang_getCString(clang_getCursorSpelling(cursor));
    if(g_Visitor->m_ConstructingNamespace.m_Classes.find(g_Visitor->m_ConstructingClass.m_ClassName) == g_Visitor->m_ConstructingNamespace.m_Classes.end())
        g_Visitor->m_ConstructingNamespace.m_Classes.emplace(g_Visitor->m_ConstructingClass.m_ClassName,g_Visitor->m_ConstructingClass);
    g_Visitor->m_ConstructingClass = ClassDecl();
}

void VisitMemberFunction(::CXCursor cursor){
    clang_visitChildren(
        cursor, 
        [](CXCursor cursor,CXCursor parent,CXClientData client_data)->CXChildVisitResult{
            CXCursorKind kind = clang_getCursorKind(cursor);
            // assume only have methods in class
            if (kind == CXCursor_ParmDecl)
                VisitParam(cursor);
            return CXChildVisit_Continue;
        } , nullptr);



    CXCursorKind kind = clang_getCursorKind(cursor);
    g_Visitor->m_ConstructingFunction.m_IsConstructor = (kind == CXCursor_Constructor);
    if(!g_FunctionName.empty() && !g_Visitor->m_ConstructingFunction.m_IsConstructor){
        std::string funcName = clang_getCString(clang_getCursorSpelling(cursor));
        if(funcName.find(g_FunctionName) != std::string::npos){
            g_Visitor->m_ConstructingFunction.m_FunctionName = g_FunctionName;
            g_Visitor->m_ConstructingClass.m_MemberMethods.emplace_back(g_Visitor->m_ConstructingFunction);
        }
        g_Visitor->m_ConstructingFunction = FunctionDecl();
    }else if(g_Visitor->m_ConstructingFunction.m_IsConstructor){
        g_Visitor->m_ConstructingFunction.m_FunctionName = clang_getCString(clang_getCursorSpelling(cursor));
        g_Visitor->m_ConstructingClass.m_MemberMethods.emplace_back(g_Visitor->m_ConstructingFunction);
        g_Visitor->m_ConstructingFunction = FunctionDecl();
    }
}

void VisitParam(CXCursor cursor){
	::CXType type = clang_getCursorType(cursor);
	g_Visitor->m_ConstructingFunction.m_ParamType.emplace_back(clang_getCString(clang_getTypeSpelling(type)));
}

void Generator(){
    std::string lF = "\n";
    std::fstream stream;
    stream.open(g_FilePath,std::ios_base::out);
    if(!stream.is_open()){
        std::printf("File Write Failed: %s",g_FilePath.c_str());
        return;
    }
    stream << "{" << lF
           << "    \"Configurations\":{" << lF
           << "        \"NameSpace\":\""<< g_NameSpace <<"\"," << lF
           << "        \"Registered\":[" << lF;
    std::unordered_map<std::string,ClassContainer> classes;
    for(uint32_t i = 0; i < g_HeaderFileCount; i++){
        Visitor visitor(g_Args,g_IncludeCount,g_HeaderFiles[i],g_Debug);
        visitor.visit(g_NameSpace);
        for(auto& valNameSpace : visitor.getAst()){
            for(auto& valClass : valNameSpace.m_Classes){
                if(classes.find(valClass.first) != classes.end())
                    continue;
                ClassContainer classContainer;
                classContainer.m_Classs = valClass.second;
                classContainer.m_FilePath =  g_HeaderFiles[i];
                classes.emplace(valClass.first,classContainer);
            }
        }
    }
    for(auto& valClass : classes){
        if(isMark)
            stream << "," << lF;
        if(!isMark)
            isMark = true;

        stream << "            {" << lF;
        std::list<std::string> m_MemberMethods;
        std::list<std::string> constructorParams;
        for(auto& valMemberMethods : valClass.second.m_Classs.m_MemberMethods){
            if(valMemberMethods.m_IsConstructor){
                for(auto& valParamType : valMemberMethods.m_ParamType){
                    std::string begin = "std::shared_ptr<";
                    std::string end = ">";
                    std::string::size_type beginPos = valParamType.find(begin);
                    std::string::size_type endPos = valParamType.find(end);
                    if(beginPos != std::string::npos){
                        beginPos += begin.size();
                        constructorParams.emplace_back(valParamType.substr(beginPos,endPos-beginPos));
                    }else if(valParamType == "const int &"){
                        std::cout << "!!!!" << std::endl;
                    }
                }
            }else{
                m_MemberMethods.emplace_back(valMemberMethods.m_FunctionName);
            }
        }
        bool isFirst = false;
        stream << "                \"Id\":\"" << valClass.second.m_Classs.m_ClassName <<"\"," << lF
                << "                \"Class\":\"" << g_NameSpace << "::" << valClass.second.m_Classs.m_ClassName << "\"," << lF
                << "                \"Interface\":\"" << valClass.second.m_Classs.m_InterfaceName << "\"," << lF
                << "                \"IncludePath\": \"" << valClass.second.m_FilePath << "\"," << lF
                << "                \"Dependencies\":[";
            for(auto& param : constructorParams){
                if(isFirst)
                    stream << ",";
                if(!isFirst)
                    isFirst = true;
                stream << "\"" << param << "\"";
            }
            isFirst = false;
            stream << "]," << lF
                    << "                \"FunctionPropertys\":[";
            for(auto& methods : m_MemberMethods){
                if(isFirst)
                    stream << ",";
                if(!isFirst)
                    isFirst = true;
                stream << "\"" << methods << "\"";
            }
        stream << "]" << lF
                << "            }";
    }


    stream  << lF
           << "        ]," << lF
           << "        \"Initialize\":[" << lF
           << "        ]," << lF
           << "        \"Constructor\":{" << lF
           << "            \"Name\":\"Initialize\"," << lF;
        if(g_WithParameter){
            stream << "            \"WithParameter\": true" << lF;
        }else{
            stream << "            \"WithParameter\": false" << lF;
        }
    stream << "        }" << lF
           << "    }" << lF
           << "}" << lF;
    stream.flush();
}

// WORKING_DIRECTORY
int main(int argc, char** argv){
    if(argc < 3){
        std::printf("Parameter Error\n");
        std::printf("\t-D Enable Debug Mode.\t");
        std::printf("\t-N Namespace To Be Parsed.\t");
        std::printf("\t-O Output Module File Name.\t");
        std::printf("\t-H Header File To Be Parsed.\t");
        std::printf("\t-F Function Convention Name To Be Parsed.\t");
        std::printf("\t-I The Header File Of The Dependent Library.\t");
        return EXIT_FAILURE;
    }
    for(auto i = 1; i< argc;i++){
        if(argv[i][0] == '-'){
            switch (argv[i][1]){
            case 'I':   /// 依赖库的头文件
                g_Args[g_IncludeCount] = argv[i];
                g_IncludeCount++;
                break;
            case 'H':  /// 需解析的头文件
                g_HeaderFiles[g_HeaderFileCount] = &(argv[i][2]);
                g_HeaderFileCount++;
                break;
            case 'N':  /// 需解析的命名空间
                g_NameSpace.assign(&(argv[i][2]));
                break;
            case 'O':  /// 输出的模块文件名
                g_FilePath.assign(&(argv[i][2]));
                g_FilePath.append(".magic");
                break;
            case 'F':  /// 需解析对应函数中的约定名
                g_FunctionName.assign(&(argv[i][2]));
                break;
            case 'D':  /// 是否需要开启调试模式
                g_Debug = true;
                break;
            case 'P':
                g_WithParameter = true;
            default:
                break;
            }
        }
    }
    bool isError = false;
    if(g_NameSpace.empty()){
        isError = true;
        std::printf("Error: Namespace Is Empty\n");
    }
    if(g_FilePath.empty()){
        isError = true;
        std::printf("Error: OutPut File Path Is Empty\n");
    }
    if(g_FunctionName.empty()){
        isError = true;
        std::printf("Error: Function Name Is Empty\n");
    }
    if(g_IncludeCount == 0){
        isError = true;
        std::printf("Error: Dependent Library Header File Is Empty \n");
    }
    if(g_HeaderFileCount == 0){
        isError = true;
        std::printf("Error: Header File Is Empty\n");
    }
    if(isError){
        return EXIT_FAILURE;
    }
    Generator();
    std::cout.flush();
    return EXIT_SUCCESS;
}