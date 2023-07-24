/*
 ******************************************************************************
 * @Author         : INotfound
 * @File           : MultiPart.hpp
 * @Date           : 2023-07-03 18:28
 ******************************************************************************
 */
#pragma once

#include <map>
#include <utility>
#include <fstream>
#include "Magic/Core/Core.hpp"
#include "Magic/NetWork/Http/Utils/MultiPartParser.hpp"

namespace Magic{
namespace NetWork{
namespace Http{
    class MultiPart:public Noncopyable{
    public:
        ~MultiPart();

        MultiPart();

        explicit MultiPart(const StringView& boundary);

        void reset();

        bool stopped() const;

        bool succeeded() const;

        bool hasError() const;

        const char* getErrorMessage() const;

        size_t feed(const char* buffer,size_t len);

        void setBoundary(const StringView& boundary);

        void setDirectory(const StringView& dirPath);

        const std::unordered_map<std::string,std::string>& getParamMap() const;

    private:
        void setParserCallBacks();

        std::string getName();

        std::string getFileName();

        static void PartBegin(const char* buffer,size_t start,size_t end,void* userData);

        static void HeaderField(const char* buffer,size_t start,size_t end,void* userData);

        static void HeaderValue(const char* buffer,size_t start,size_t end,void* userData);

        static void HeaderEnd(const char* buffer,size_t start,size_t end,void* userData);

        static void HeadersEnd(const char* buffer,size_t start,size_t end,void* userData);

        static void PartData(const char* buffer,size_t start,size_t end,void* userData);

        static void PartEnd(const char* buffer,size_t start,size_t end,void* userData);

        static void End(const char* buffer,size_t start,size_t end,void* userData);

    private:
        std::string m_Boundary;
        std::string m_FilePath;
        std::string m_Directory;
        std::string m_ParamName;
        std::string m_HeaderName;
        MultiPartParser m_Parser;
        std::string m_ParamValue;
        std::string m_HeaderValue;
        std::ofstream m_FileStream;
        std::vector<std::string> m_FilePaths;
        std::unordered_map<std::string,std::string> m_ParamMap;
        std::unordered_map<std::string,std::string> m_HeaderMap;
        std::vector<std::unordered_map<std::string,std::string>> m_HeaderMaps;
    };

}
}
}

