/*
 * @Author: INotFound
 * @Date: 2020-09-12 16:23:53
 * @LastEditTime: 2023-05-02 15:21:32
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
    template<typename T,typename = typename std::enable_if<std::is_same<T,std::string>::value || std::is_same<T,std::string_view>::value || std::is_integral<T>::value || std::is_floating_point<T>::value>::type>
    inline T StringAs(const std::string_view& value){
        T newValue = {};
        std::istringstream stringStream(std::string(value.data(),value.size()));
        stringStream >> std::dec >> newValue;
        if(stringStream.fail()){
            throw Failure("Invalid Argument!");
        }
        return newValue;
    }

    template<>
    inline bool StringAs<bool>(const std::string_view& value){
        bool isOk = true;
        std::string tValue;
        std::transform(value.begin(),value.end(),std::back_inserter(tValue),::toupper);
        if(tValue == "FALSE" || tValue == "NO" || tValue == "0")
            isOk = false;
        return isOk;
    }

    /**
     * @warning Use Exception!
     */
    template<>
    inline float StringAs<float>(const std::string_view& value){
        float newValue = {};
        std::istringstream stringStream(std::string(value.data(),value.size()));
        stringStream >> newValue;
        if(stringStream.fail()){
            throw Failure("Invalid Argument!");
        }
        return newValue;
    }

    /**
     * @warning Use Exception!
     */
    template<>
    inline double StringAs<double>(const std::string_view& value){
        double newValue = {};
        std::istringstream stringStream(std::string(value.data(),value.size()));
        stringStream >> newValue;
        if(stringStream.fail()){
            throw Failure("Invalid Argument!");
        }
        return newValue;
    }

    template<>
    inline std::string StringAs<std::string>(const std::string_view& value){
        return std::string(value.data(),value.size());
    }

    template<>
    inline std::string_view StringAs<std::string_view>(const std::string_view& value){
        return value;
    }

    /**
     * @warning Use Exception!
     */
    template<typename T,typename = typename std::enable_if<std::is_same<T,std::string>::value || std::is_same<T,std::string_view>::value || std::is_integral<T>::value>::type>
    inline T StringAs(const std::string_view& value,uint8_t base){
        T newValue = {};
        std::istringstream stringStream(std::string(value.data(),value.size()));
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
            throw Failure("Invalid Argument!");
        }
        return newValue;
    }

    /**
     * @warning Use Exception!
     */
    template<>
    inline int8_t StringAs<int8_t>(const std::string_view& value,uint8_t base){
        int16_t newValue = {};
        std::istringstream stringStream(std::string(value.data(),value.size()));
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
            throw Failure("Invalid Argument!");
        }
        return static_cast<int8_t>(newValue);
    }

    template<>
    inline std::string StringAs<std::string>(const std::string_view& value,uint8_t){
        return std::string(value.data(),value.size());
    }

    template<>
    inline std::string_view StringAs<std::string_view>(const std::string_view& value,uint8_t){
        return value;
    }

    /**
     * @warning Use Exception!
     */
    template<>
    inline uint8_t StringAs<uint8_t>(const std::string_view& value,uint8_t base){
        uint16_t newValue = {};
        std::istringstream stringStream(std::string(value.data(),value.size()));
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
            throw Failure("Invalid Argument!");
        }
        return static_cast<uint8_t>(newValue);
    }

    inline std::string ToLower(const std::string_view& string){
        std::string newString;
        std::transform(string.begin(),string.end(),std::back_inserter(newString),::tolower);
        return newString;
    }

    inline std::string ToUpper(const std::string_view& string){
        std::string newString;
        std::transform(string.begin(),string.end(),std::back_inserter(newString),::toupper);
        return newString;
    }

    inline std::string Trim(const std::string_view& str,const std::string_view& flag = " "){
        std::string string(str.data(),str.size());
        if(str.empty())
            return g_EmptyString;
        string.erase(0,string.find_first_not_of(flag.data(),flag.size()));
        string.erase(string.find_last_not_of(flag.data(),flag.size()) + 1);
        return string;
    }

    int32_t StringCompareCase(const std::string_view& dest,const std::string_view& src);

    int32_t StringCompareNoCase(const std::string_view& dest,const std::string_view& src);

    inline std::vector<std::string_view> Split(const std::string_view& str,const std::string_view& delim){
        std::size_t previous = 0;
        std::size_t current = str.find(delim);
        std::vector<std::string_view> elems;
        while(current != std::string_view::npos){
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

    inline std::string_view SubString(const std::string_view& str,uint64_t index,const std::string_view& delim){
        uint64_t pos = str.find(delim,index);
        if(pos == std::string::npos)
            return str.substr(index,str.size() - index);;
        return str.substr(index,pos - index);
    }

    inline std::string TimeToString(std::time_t ts,const std::string_view& format = "%Y-%m-%d %H:%M:%S"){
        struct tm nowTime;
    #if defined(_WIN32) || defined(_WIN64)
        localtime_s(&nowTime, &ts);
    #else
        localtime_r(&ts,&nowTime);
    #endif
        char buf[512] = {0};
        std::strftime(buf,sizeof(buf),format.data(),&nowTime);
        return buf;
    }

    inline std::string Replace(const std::string_view& string,const std::string_view& oldStr,const std::string_view& newStr){
        std::string newString = std::string(string.data(),string.size());
        for(std::string::size_type pos = 0;pos != std::string::npos;pos += newStr.length()){
            if((pos = newString.find(oldStr.data(),oldStr.size(),pos)) != std::string::npos){
                newString.replace(pos,oldStr.size(),newStr.data(),newStr.size());
            }else{
                return newString;
            }
        }
        return newString;
    }

    class CaseInsensitiveLess{
    public:
        bool operator()(const std::string& lhs,const std::string& rhs) const{
            return lhs.compare(rhs) < 0;
        }

        bool operator()(const std::string_view& lhs,const std::string_view& rhs) const{
            return lhs.compare(rhs) < 0;
        }
    };

}