#pragma once
#include <unordered_map>
#include "Core.h"
#include "Macro.h"
#include "Mutex.h"
#include "Http/Http.h"
namespace Magic{
namespace Http{
namespace Instance{
    class HttpCache{
    public:
        HttpCache();
        void Init(const std::string& resourcePath);
        const std::string& getCache(const std::string& path);
    private:
        bool m_IsInit;
        RWMutex m_Mutex;
        std::string m_ResourcePath;
        std::unordered_map<std::string,std::string> m_Resource;
    };
}
    typedef SingletonPtr<Instance::HttpCache> HttpStaticCache;
}
}
