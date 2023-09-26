/*
 ******************************************************************************
 * @Author         : INotfound
 * @File           : MultiPart.cpp
 * @Date           : 2023-07-03 18:33
 ******************************************************************************
 */
#include "Magic/Utilty/Crypto.hpp"
#include "Magic/Utilty/String.hpp"
#include "Magic/Core/StringView.hpp"
#include "Magic/NetWork/Http/MultiPart.hpp"

namespace Magic{
namespace NetWork{
namespace Http{
    MultiPart::~MultiPart(){
        this->reset();
    }

    MultiPart::MultiPart(){
        setParserCallBacks();
    }

    MultiPart::MultiPart(const StringView& boundary)
        :m_Boundary(boundary.data(),boundary.size())
        ,m_Parser(boundary){
        setParserCallBacks();
    }

    StringView MultiPart::getParam(const StringView& key) const{
        auto iter = m_ParamMap.find(std::string(key.data(),key.size()));
        if(iter == m_ParamMap.end()){
            return g_EmptyString;
        }
        return iter->second;
    }

    const Safe<FileStream>& MultiPart::getFile(const StringView& sv){
        auto iter = m_Files.find(std::string(sv.data(),sv.size()));
        if(iter == m_Files.end()){
            return m_FileStream;
        }
        return iter->second;
    }

    const std::unordered_map<std::string,Safe<FileStream>>& MultiPart::getFiles() const{
        return m_Files;
    }

    void MultiPart::reset(){
        for(const auto& v : m_Files){
            if(v.second){
                v.second->remove();
            }
        }
        if(m_FileStream){
            m_FileStream->remove();
        }
        m_FileStream.reset();
        m_HeaderMaps.clear();
        m_HeaderMap.clear();
        m_ParamMap.clear();
        m_Parser.reset();
        m_Files.clear();
    }

    bool MultiPart::stopped() const{
        return m_Parser.stopped();
    }

    StringView MultiPart::getName(){
        auto it = m_HeaderMap.find("Content-Disposition");
        if(it != m_HeaderMap.end()){
            auto pos = it->second.find("name");
            if(pos != std::string::npos){
                auto start = it->second.find('"',pos) + 1;
                auto end = it->second.find('"',start + 1);
                if(start != std::string::npos || end != std::string::npos || end > start){
                    return StringView(it->second.data() + start,end - start);
                }
            }
        }
        return StringView();
    }

    bool MultiPart::hasError() const{
        return m_Parser.hasError();
    }

    bool MultiPart::succeeded() const{
        return m_Parser.succeeded();
    }

    StringView MultiPart::getFileName(){
        auto it = m_HeaderMap.find("Content-Disposition");
        if(it != m_HeaderMap.end()){
            auto pos = it->second.find("filename");
            if(pos != std::string::npos){
                auto start = it->second.find('"',pos) + 1;
                auto end = it->second.find('"',start + 1);
                if(start != std::string::npos || end != std::string::npos || end > start){
                    return StringView(it->second.data() + start,end - start);
                }
            }
        }
        return StringView();
    }

    void MultiPart::setParserCallBacks(){
        m_Parser.onPartBegin = PartBegin;
        m_Parser.onHeaderField = HeaderField;
        m_Parser.onHeaderValue = HeaderValue;
        m_Parser.onHeaderEnd = HeaderEnd;
        m_Parser.onHeadersEnd = HeadersEnd;
        m_Parser.onPartData = PartData;
        m_Parser.onPartEnd = PartEnd;
        m_Parser.onEnd = End;
        m_Parser.userData = this;
    }

    const char* MultiPart::getErrorMessage() const{
        return m_Parser.getErrorMessage();
    }

    void MultiPart::setBoundary(const StringView& boundary){
        m_Boundary = std::string(boundary.data(),boundary.size());
        m_Parser.setBoundary(boundary);
    }

    void MultiPart::setDirectory(const StringView& dirPath){
        m_Directory = std::string(dirPath.data(),dirPath.size());
    }

    size_t MultiPart::feed(const IStream::BufferView bufferView){
        return m_Parser.feed(bufferView.data(),bufferView.size());
    }

    void MultiPart::PartBegin(const char*/*buffer*/,size_t /*start*/,size_t /*end*/,void* userData){
        auto* self = reinterpret_cast<MultiPart*>(userData);
        self->m_FilePath.clear();
        self->m_ParamName.clear();
        self->m_ParamValue.clear();
        self->m_HeaderName.clear();
        self->m_HeaderValue.clear();
    }

    void MultiPart::HeaderField(const char* buffer,size_t start,size_t end,void* userData){
        auto* self = reinterpret_cast<MultiPart*>(userData);
        self->m_HeaderName.append(buffer + start,end - start);
    }

    void MultiPart::HeaderValue(const char* buffer,size_t start,size_t end,void* userData){
        auto* self = reinterpret_cast<MultiPart*>(userData);
        self->m_HeaderValue.append(buffer + start,end - start);
    }

    void MultiPart::HeaderEnd(const char*/*buffer*/,size_t /*start*/,size_t /*end*/,void* userData){
        auto* self = reinterpret_cast<MultiPart*>(userData);
        self->m_HeaderMap.insert(std::make_pair(self->m_HeaderName,self->m_HeaderValue));
        self->m_HeaderName.clear();
        self->m_HeaderValue.clear();
    }

    void MultiPart::HeadersEnd(const char*/*buffer*/,size_t /*start*/,size_t /*end*/,void* userData){
        auto* self = reinterpret_cast<MultiPart*>(userData);
        self->m_HeaderName.clear();
        self->m_HeaderValue.clear();
        self->m_HeaderMaps.push_back(self->m_HeaderMap);
        /// TODO: PartBegin
        auto fileName = self->getFileName();
        auto name = self->getName();
        if(!fileName.empty()){
            auto ext = fileName.substr(fileName.rfind('.'));
            Base64Encoder encoder(std::make_shared<DataStream>(Magic::StringCat(self->m_Boundary,fileName)));
            auto fileStream = std::make_shared<FileStream>(Magic::StringCat(self->m_Directory,"/",encoder.read(),ext));
            if(fileStream->open(FileStream::OpenMode::ReadWrite)){
                self->m_FileStream = std::move(fileStream);
            }
        }
        if(!name.empty()){
            self->m_ParamName = std::string(name.data(),name.size());
        }
        self->m_HeaderMap.clear();
    }

    void MultiPart::PartData(const char* buffer,size_t start,size_t end,void* userData){
        auto* self = reinterpret_cast<MultiPart*>(userData);
        if(self->m_FileStream){
            self->m_FileStream->write(FileStream::BufferView(buffer + start,end - start));
        }else if(!self->m_ParamName.empty()){
            self->m_ParamValue.append(buffer + start,end - start);
        }
    }

    void MultiPart::PartEnd(const char*/*buffer*/,size_t /*start*/,size_t /*end*/,void* userData){
        auto* self = reinterpret_cast<MultiPart*>(userData);
        if(!self->m_ParamName.empty()){
            if(self->m_FileStream){
                self->m_Files.emplace(self->m_ParamName,self->m_FileStream);
            }else{
                self->m_ParamMap.emplace(self->m_ParamName,self->m_ParamValue);
            }
        }
        self->m_FileStream.reset();
        self->m_ParamValue.clear();
        self->m_ParamName.clear();
    }

    void MultiPart::End(const char*/*buffer*/,size_t /*start*/,size_t /*end*/,void* userData){
        auto* self = reinterpret_cast<MultiPart*>(userData);
        self->m_HeaderMaps.push_back(self->m_HeaderMap);
        self->m_HeaderMap.clear();
    }
}
}
}