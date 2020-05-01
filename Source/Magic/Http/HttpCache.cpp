/*
 * @File: HttpCache.cpp
 * @Author: INotFound
 * @Date: 2020-02-20 10:17:54
 * @LastEditTime: 2020-04-05 23:42:29
 */
#include <vector>
#include <fstream>
#include "Gzip.h"
#include "Http/HttpCache.h"
namespace Magic{
namespace Http{
namespace Instance{
    HttpCache::HttpCache()
        :m_IsInit(false){
    }
    void HttpCache::init(const std::string& resourcePath){
        if(m_IsInit)
            return;
        RWMutex::WriteLock lock(m_Mutex);
        m_IsInit = true;
        std::vector<std::string> vec;
        Magic::ReadFileList(resourcePath,vec);
        std::ofstream stream;
        std::ostringstream staticRes;
        static auto strFunc = [](unsigned char c) -> unsigned char{
            if(c == '\\'){
                return '/';
            }
            return c;
        };
        std::string compressData;
        for(auto& i : vec){
            stream.open(i,std::ios::in);
            std::ostringstream staticRes;
            staticRes << stream.rdbuf();
            std::string subPath = i.substr(resourcePath.size());
            std::transform(subPath.begin(), subPath.end(), subPath.begin(), strFunc);
            Gzip::Compress(staticRes.str(),compressData);
            m_Resource.emplace(subPath,compressData);
            stream.close();
            compressData.clear();
        }
    }
    const std::string& HttpCache::getCache(const std::string& path){
        RWMutex::ReadLock lock(m_Mutex);
        auto iter = m_Resource.find(path);
        if(iter != m_Resource.end()){
            return iter->second;
        }
        return g_EmptyString;
    }
}
}
}