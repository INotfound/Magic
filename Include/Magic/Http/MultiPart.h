#pragma once
#include "Core.h"
#include "Http/Http.h"
#include "Http/HttpFile.h"
namespace Magic{
namespace Http{
    class MultiPart
    {
    public:
        ~MultiPart();
        MultiPart();
        bool parse(const Safe<HttpRequest>& request);
        const std::vector<Safe<HttpFile>>& getFiles() const;
        const std::map<std::string,std::string>& getParams() const;
    private:
        bool parseEntity(const char* begin,const char* end);
    private:
        std::vector<Safe<HttpFile>> m_Files;
        std::map<std::string,std::string> m_Params;
    };
}
}