/*
 * @Author: INotFound
 * @Date: 2020-09-12 16:23:53
 * @LastEditTime: 2020-09-30 21:29:32
 */
#pragma once

#include <ctime>
#include <cctype>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include <type_traits>
#include "Magic/Core/Core.h"

namespace Magic{
    static const std::string g_EmptyString;

    template<typename T>
    std::string AsString(const T& value){
        std::ostringstream formatStream;
        formatStream << value;
        return formatStream.str();
    }

    /**
     * @warning Use Exception!
     */
    template<typename T,typename = typename std::enable_if<std::is_same<T,std::string>::value || std::is_integral<T>::value || std::is_floating_point<T>::value>::type>
    inline T StringAs(const std::string& value){
        T newValue = {};
        std::istringstream stringStream(value);
        stringStream >> std::dec >> newValue;
        if(stringStream.fail()){
            throw std::invalid_argument(value);
        }
        return newValue;
    }

    template<>
    inline bool StringAs<bool>(const std::string& value){
        bool isOk = true;
        std::string tValue;
        std::transform(value.begin(),value.end(),std::back_inserter(tValue),::toupper);
        if(tValue == std::string("FALSE") || tValue == std::string("NO") || tValue == std::string("0"))
            isOk = false;
        return isOk;
    }

    /**
     * @warning Use Exception!
     */
    template<>
    inline float StringAs<float>(const std::string& value){
        float newValue = {};
        std::istringstream stringStream(value);
        stringStream >> newValue;
        if(stringStream.fail()){
            throw std::invalid_argument(value);
        }
        return newValue;
    }

    /**
     * @warning Use Exception!
     */
    template<>
    inline double StringAs<double>(const std::string& value){
        double newValue = {};
        std::istringstream stringStream(value);
        stringStream >> newValue;
        if(stringStream.fail()){
            throw std::invalid_argument(value);
        }
        return newValue;
    }

    template<>
    inline std::string StringAs<std::string>(const std::string& value){
        return value;
    }

    /**
     * @warning Use Exception!
     */
    template<typename T,typename = typename std::enable_if<std::is_same<T,std::string>::value || std::is_integral<T>::value>::type>
    inline T StringAs(const std::string& value,uint8_t base){
        T newValue = {};
        std::istringstream stringStream(value);
        switch(base){
            case 8:
                stringStream >> std::oct >> newValue;
                break;
            case 10:
                stringStream >> std::dec >> newValue;
                break;
            case 16:
                stringStream >> std::hex >> newValue;
                break;
            default:
                stringStream >> std::dec >> newValue;
                break;
        }
        if(stringStream.fail()){
            throw std::invalid_argument(value);
        }
        return newValue;
    }

    /**
     * @warning Use Exception!
     */
    template<>
    inline int8_t StringAs<int8_t>(const std::string& value,uint8_t base){
        int16_t newValue = {};
        std::istringstream stringStream(value);
        switch(base){
            case 8:
                stringStream >> std::oct >> newValue;
                break;
            case 10:
                stringStream >> std::dec >> newValue;
                break;
            case 16:
                stringStream >> std::hex >> newValue;
                break;
            default:
                stringStream >> std::dec >> newValue;
                break;
        }
        if(stringStream.fail() || (newValue > 0xFF)){
            throw std::invalid_argument(value);
        }
        return static_cast<int8_t>(newValue);
    }

    template<>
    inline std::string StringAs<std::string>(const std::string& value,uint8_t){
        return value;
    }

    /**
     * @warning Use Exception!
     */
    template<>
    inline uint8_t StringAs<uint8_t>(const std::string& value,uint8_t base){
        uint16_t newValue = {};
        std::istringstream stringStream(value);
        switch(base){
            case 8:
                stringStream >> std::oct >> newValue;
                break;
            case 10:
                stringStream >> std::dec >> newValue;
                break;
            case 16:
                stringStream >> std::hex >> newValue;
                break;
            default:
                stringStream >> std::dec >> newValue;
                break;
        }
        if(stringStream.fail() || (newValue > 0xFF)){
            throw std::invalid_argument(value);
        }
        return static_cast<uint8_t>(newValue);
    }

    inline std::string TimeToGMTString(std::time_t tt){
        struct tm* GMTime = gmtime(&tt);
        char buff[512] = {0};
        std::strftime(buff,sizeof(buff),"%a, %d %b %Y %H:%M:%S %Z",GMTime);
        return buff;
    }

    inline std::string ToLower(const std::string& string){
        std::string newString;
        std::transform(string.begin(),string.end(),std::back_inserter(newString),::tolower);
        return newString;
    }

    inline std::string ToUpper(const std::string& string){
        std::string newString;
        std::transform(string.begin(),string.end(),std::back_inserter(newString),::toupper);
        return newString;
    }

    inline std::string Trim(std::string str,const std::string& flag = " "){
        if(str.empty())
            return str;
        str.erase(0,str.find_first_not_of(flag));
        str.erase(str.find_last_not_of(flag) + 1);
        return str;
    }

    int32_t StringCompareCase(const std::string& dest,const std::string& src);

    int32_t StringCompareNoCase(const std::string& dest,const std::string& src);

    inline std::vector<std::string> Split(const std::string& str,const std::string& delim){
        std::size_t previous = 0;
        std::size_t current = str.find(delim);
        std::vector<std::string> elems;
        while(current != std::string::npos){
            if(current > previous){
                elems.push_back(str.substr(previous,current - previous));
            }
            previous = current + 1;
            current = str.find(delim,previous);
        }
        if(previous != str.size()){
            elems.push_back(str.substr(previous));
        }
        return elems;
    }

    inline std::string TimeToString(std::time_t ts,const std::string& format = "%Y-%m-%d %H:%M:%S"){
        struct tm nowTime;
    #if defined(_WIN32) || defined(_WIN64)
        localtime_s(&nowTime, &ts);
    #else
        localtime_r(&ts,&nowTime);
    #endif
        char buf[64] = {0};
        std::strftime(buf,sizeof(buf),format.data(),&nowTime);
        return buf;
    }

    inline std::string SubString(const std::string& str,uint64_t index,const std::string& delim){
        uint64_t pos = str.find(delim,index);
        if(pos == std::string::npos)
            return str.substr(index,str.size() - index);;
        return str.substr(index,pos - index);
    }

    inline std::string Replace(const std::string& string,const std::string& oldStr,const std::string& newStr){
        std::string newString = string;
        for(std::string::size_type pos = 0;pos != std::string::npos;pos += newStr.length()){
            if((pos = newString.find(oldStr,pos)) != std::string::npos){
                newString.replace(pos,oldStr.length(),newStr);
            }else{
                return newString;
            }
        }
        return newString;
    }

    class CaseResponsiveLess{
    public:
        bool operator()(const std::string& lhs,const std::string& rhs) const{
            return StringCompareCase(lhs,rhs) < 0;
        }
    };

    class CaseInsensitiveLess{
    public:
        bool operator()(const std::string& lhs,const std::string& rhs) const{
            return StringCompareNoCase(lhs,rhs) < 0;
        }
    };

}