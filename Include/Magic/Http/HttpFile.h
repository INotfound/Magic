#pragma once
#include <fstream>
namespace Magic{
namespace Http{

    class HttpFile{
    public:
        ~HttpFile();
        HttpFile(const std::string& name,const std::string& content);
        bool save(const std::string& path);
        const std::string getName() const;
    private:
        std::string m_Name;
        std::string m_Content;
        std::ofstream m_FileStream;
    };

}
}