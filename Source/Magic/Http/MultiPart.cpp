/*
 * @file: MultiPart.cpp
 * @Author: INotFound
 * @Date: 2020-03-11 00:20:52
 * @LastEditTime: 2020-03-15 17:56:55
 */
#include "Http/MultiPart.h"

namespace Magic{
namespace Http{
    MultiPart::~MultiPart(){
    }
    MultiPart::MultiPart(){
    }
    bool MultiPart::parse(const Safe<HttpRequest>& request){
        const std::string& contentType = request->getHeader("Content-Type");
        if(contentType.empty()){
            return false;
        }
        uint64_t pos = contentType.find("boundary=");
        if(pos == std::string::npos){
            return false;
        }
        std::string boundary = contentType.substr(pos + 9);

        const std::string& content = request->getBody();
        uint64_t pos1 = 0;
        uint64_t pos2 = 0;
        pos2 = content.find(boundary);
        while (true){
            pos1 = pos2;
            if(pos1 == std::string::npos){
                return false;
            }
            pos1 += boundary.length();
            if(content.at(pos1) == '\r' && content.at(pos2) == '\n'){
                pos1 += 2;
            }
            pos2 = content.find(boundary,pos1);
            pos2 = content.rfind("\r\n",pos2);
            if(pos2 == std::string::npos){
                return false;
            }
            if(content.at(pos2 - 2) == '\r'
                &&content.at(pos2 - 1) == '\n'){
                pos2 -= 2;
            }
            if(!this->parseEntity(content.data() + pos1,content.data() + pos2)){
                return false;
            }
        }
        return true;
    }
    const std::vector<Safe<HttpFile>>& MultiPart::getFiles() const{
        return m_Files;
    }
    const std::map<std::string,std::string>& MultiPart::getParams() const{
        return m_Params;
    }
    bool MultiPart::parseEntity(const char* begin,const char* end){
        static const char EntityName[] = "name=\"";
        static const char QuotationMark[] = "\"";
        static const char FileName[] = "filename=\"";
        static const char CRLF[] = "\r\n\r\n";
        auto pos1 = std::search(begin,end,EntityName,EntityName + 6);
        if(pos1 == end){
            return false;
        }
        pos1 += 6;
        auto pos2 = std::search(pos1,end,QuotationMark,QuotationMark + 1);
        if(pos2 == end){
            return false;
        }
        std::string name(pos1,pos2);
        pos1 = std::search(pos2,end,FileName,FileName + 10);
        if(pos1 == end){
            pos2 = std::search(pos2,end,CRLF,CRLF + 4);
            if(pos2 == end){
                return false;
            }
            m_Params.emplace(name,std::string(pos2 + 4,end));
        }else{
            pos1 += 10;
            pos2 = std::search(pos1,end,QuotationMark,QuotationMark + 1);
            if(pos2 == end){
                return false;
            }
            std::string fileName(pos1,pos2);
            pos2 = std::search(pos2,end,CRLF,CRLF + 4);
            if(pos2 == end){
                return false;
            }
            Safe<HttpFile> file(new HttpFile(name,std::string(pos2 + 4,end)));
            m_Files.push_back(std::move(file));
            return true;
        }
        return false;
    }
}
}