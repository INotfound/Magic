/*
 ******************************************************************************
 * @Author         : INotfound
 * @File           : MultiPart.cpp
 * @Date           : 2023-07-03 18:33
 ******************************************************************************
 */
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

    MultiPart::MultiPart(const Magic::StringView& boundary)
        :m_Boundary(boundary.data(),boundary.size())
        ,m_Parser(boundary){
        setParserCallBacks();
    }

    void MultiPart::reset(){
        for(const auto& path : m_FilePaths){
            std::remove(path.c_str());
        }
        m_HeaderMaps.clear();
        m_HeaderMap.clear();
        m_FilePaths.clear();
        m_ParamMap.clear();
        m_Parser.reset();
    }

    bool MultiPart::stopped() const{
        return m_Parser.stopped();
    }

    bool MultiPart::hasError() const{
        return m_Parser.hasError();
    }

    bool MultiPart::succeeded() const{
        return m_Parser.succeeded();
    }

    const char* MultiPart::getErrorMessage() const{
        return m_Parser.getErrorMessage();
    }

    size_t MultiPart::feed(const char* buffer,size_t len){
        return m_Parser.feed(buffer,len);
    }

    void MultiPart::setBoundary(const Magic::StringView& boundary){
        m_Boundary = std::string(boundary.data(),boundary.size());
        m_Parser.setBoundary(boundary);
    }

    void MultiPart::setDirectory(const Magic::StringView& dirPath){
        m_Directory = std::string(dirPath.data(),dirPath.size());
    }

    const std::unordered_map<std::string,std::string>& MultiPart::getParamMap() const{
        return m_ParamMap;
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

    std::string MultiPart::getName(){
        auto it = m_HeaderMap.find("Content-Disposition");
        if(it != m_HeaderMap.end()){
            auto pos = it->second.find("name");
            if(pos != std::string::npos){
                auto start = it->second.find('"',pos) + 1;
                auto end = it->second.find('"',start + 1);
                if(start != std::string::npos || end != std::string::npos || end > start){
                    return it->second.substr(start,end - start);
                }
            }
        }
        return std::string();
    }

    std::string MultiPart::getFileName(){
        auto it = m_HeaderMap.find("Content-Disposition");
        if(it != m_HeaderMap.end()){
            auto pos = it->second.find("filename");
            if(pos != std::string::npos){
                auto start = it->second.find('"',pos) + 1;
                auto end = it->second.find('"',start + 1);
                if(start != std::string::npos || end != std::string::npos || end > start){
                    return it->second.substr(start,end - start);
                }
            }
        }
        return std::string();
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
        std::string fileName = self->getFileName();
        if(!fileName.empty()){
            self->m_FilePath = self->m_Directory + '/' + self->m_Boundary + fileName.substr(fileName.rfind('.'));
            self->m_FileStream.open(self->m_FilePath,std::ios::binary);
        }

        std::string name = self->getName();
        if(!name.empty()){
            self->m_ParamName = name;
        }

        self->m_HeaderMap.clear();
    }

    void MultiPart::PartData(const char* buffer,size_t start,size_t end,void* userData){
        auto* self = reinterpret_cast<MultiPart*>(userData);
        if(self->m_FileStream.is_open()){
            self->m_FileStream.write(buffer + start,end - start);
        }else if(!self->m_ParamName.empty()){
            self->m_ParamValue.append(buffer + start,end - start);
        }
    }

    void MultiPart::PartEnd(const char*/*buffer*/,size_t /*start*/,size_t /*end*/,void* userData){
        auto* self = reinterpret_cast<MultiPart*>(userData);
        if(!self->m_ParamName.empty()){
            if(self->m_FileStream.is_open()){
                self->m_ParamMap.emplace(self->m_ParamName,self->m_FilePath);
                self->m_FilePaths.push_back(self->m_FilePath);
                self->m_FileStream.flush();
                self->m_FileStream.close();
                self->m_FilePath.clear();
            }else{
                self->m_ParamMap.emplace(self->m_ParamName,self->m_ParamValue);
            }
            self->m_ParamName.clear();
            self->m_ParamValue.clear();
        }
    }

    void MultiPart::End(const char*/*buffer*/,size_t /*start*/,size_t /*end*/,void* userData){
        auto* self = reinterpret_cast<MultiPart*>(userData);
        self->m_HeaderMaps.push_back(self->m_HeaderMap);
        self->m_HeaderMap.clear();
    }
}
}
}