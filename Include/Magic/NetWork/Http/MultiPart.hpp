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
#include "Magic/Core/Core.hpp"
#include "Magic/Core/Stream.hpp"
#include "Magic/Core/StringView.hpp"
#include "Magic/NetWork/Http/Utils/MultiPartParser.hpp"

namespace Magic{
namespace NetWork{
namespace Http{
    class MultiPart :public Noncopyable{
        friend class HttpSocket;
    public:
        ~MultiPart();

        MultiPart();

        explicit MultiPart(const StringView& boundary);

        StringView getParam(const StringView& key) const;

        const Safe<FileStream>& getFile(const StringView& sv);

        const std::unordered_map<std::string,Safe<FileStream>>& getFiles() const;

    private:
        void reset();

        bool stopped() const;

        StringView getName();

        bool hasError() const;

        bool succeeded() const;

        StringView getFileName();

        void setParserCallBacks();

        const char* getErrorMessage() const;

        void setBoundary(const StringView& boundary);

        void setDirectory(const StringView& dirPath);

        size_t feed(const IStream::BufferView bufferView);

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
        Safe<FileStream> m_FileStream;
        std::unordered_map<std::string,std::string> m_ParamMap;
        std::unordered_map<std::string,std::string> m_HeaderMap;
        std::unordered_map<std::string,Safe<FileStream>> m_Files;
        std::vector<std::unordered_map<std::string,std::string>> m_HeaderMaps;
    };

}
}
}

