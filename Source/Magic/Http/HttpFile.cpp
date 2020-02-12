#include "Http/HttpFile.h"

namespace Magic{
namespace Http{
    HttpFile::~HttpFile(){
    }
    HttpFile::HttpFile(const std::string& name,const std::string& content)
        :m_Name(name),m_Content(content){
    }
    bool HttpFile::save(const std::string& path){
        if(m_FileStream.is_open()){
            m_FileStream.close();
        }
        m_FileStream.open(path,std::ios::out);
        if(m_FileStream.is_open()){
            m_FileStream << m_Content;
            m_FileStream.flush();
            return true;
        }
        return false;
    }
    const std::string HttpFile::getName() const{
        return m_Name;
    }
}
}