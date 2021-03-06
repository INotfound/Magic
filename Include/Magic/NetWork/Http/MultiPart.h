#pragma once
#include <map>
#include <utility>
#include <fstream>
#include "Magic/Core/Core.h"
#include "Magic/NetWork/Http/Utils/MultiPartParser.h"

namespace Magic{
namespace NetWork{
namespace Http{
    class MultiPart{
    public:
        MultiPart();

        MultiPart(const std::string &boundary);

        void reset();

        bool stopped() const;

        bool succeeded() const;

        bool hasError() const;

        const char* getErrorMessage() const;

        size_t feed(const char *buffer, size_t len);

        void setBoundary(const std::string &boundary);

        void setTempDirectory(const std::string& dirPath);
    private:
        void setParserCallbacks();

        std::string getFileName();

        static void PartBegin(const char *buffer, size_t start, size_t end, void *userData);

        static void HeaderField(const char *buffer, size_t start, size_t end, void *userData);

        static void HeaderValue(const char *buffer, size_t start, size_t end, void *userData);

        static void HeaderEnd(const char *buffer, size_t start, size_t end, void *userData);

        static void HeadersEnd(const char *buffer, size_t start, size_t end, void *userData);

        static void PartData(const char *buffer, size_t start, size_t end, void *userData);

        static void PartEnd(const char *buffer, size_t start, size_t end, void *userData);

        static void End(const char *buffer, size_t start, size_t end, void *userData);
    private:
        bool m_IsFile;
        std::string m_FilePath;
        std::string m_HeaderName;
        MultiPartParser m_Parser;
        std::string m_HeaderValue;
        std::ofstream m_FileStream;
        std::string m_TempDirectory;
        std::vector<std::string> m_FilePaths;
        std::unordered_map<std::string,std::string> m_HeaderMap;
        std::vector<std::unordered_map<std::string,std::string>> m_HeaderMaps;
    };

}
}
}

