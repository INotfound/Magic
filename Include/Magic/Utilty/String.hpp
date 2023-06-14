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
#include "Magic/Core/Core.hpp"

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
    template<typename T,typename = typename std::enable_if<std::is_same<T,std::string>::value || std::is_same<T,Magic::StringView>::value || std::is_integral<T>::value || std::is_floating_point<T>::value>::type>
    inline T StringAs(const Magic::StringView& value){
        T newValue = {};
        std::istringstream stringStream(std::string(value.data(),value.size()));
        stringStream >> std::dec >> newValue;
        if(stringStream.fail()){
            throw Failure("Invalid Argument!");
        }
        return newValue;
    }

    template<>
    inline bool StringAs<bool>(const Magic::StringView& value){
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
    inline float StringAs<float>(const Magic::StringView& value){
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
    inline double StringAs<double>(const Magic::StringView& value){
        double newValue = {};
        std::istringstream stringStream(std::string(value.data(),value.size()));
        stringStream >> newValue;
        if(stringStream.fail()){
            throw Failure("Invalid Argument!");
        }
        return newValue;
    }

    template<>
    inline std::string StringAs<std::string>(const Magic::StringView& value){
        return std::string(value.data(),value.size());
    }

    template<>
    inline Magic::StringView StringAs<Magic::StringView>(const Magic::StringView& value){
        return value;
    }

    /**
     * @warning Use Exception!
     */
    template<typename T,typename = typename std::enable_if<std::is_same<T,std::string>::value || std::is_same<T,Magic::StringView>::value || std::is_integral<T>::value>::type>
    inline T StringAs(const Magic::StringView& value,uint8_t base){
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
    inline int8_t StringAs<int8_t>(const Magic::StringView& value,uint8_t base){
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
    inline std::string StringAs<std::string>(const Magic::StringView& value,uint8_t){
        return std::string(value.data(),value.size());
    }

    template<>
    inline Magic::StringView StringAs<Magic::StringView>(const Magic::StringView& value,uint8_t){
        return value;
    }

    /**
     * @warning Use Exception!
     */
    template<>
    inline uint8_t StringAs<uint8_t>(const Magic::StringView& value,uint8_t base){
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

    inline std::string ToLower(const Magic::StringView& string){
        std::string newString;
        std::transform(string.begin(),string.end(),std::back_inserter(newString),::tolower);
        return newString;
    }

    inline std::string ToUpper(const Magic::StringView& string){
        std::string newString;
        std::transform(string.begin(),string.end(),std::back_inserter(newString),::toupper);
        return newString;
    }

    inline std::string Trim(const Magic::StringView& str,const Magic::StringView& flag = " "){
        std::string string(str.data(),str.size());
        if(str.empty())
            return g_EmptyString;
        string.erase(0,string.find_first_not_of(std::string(flag.data(),flag.size())));
        string.erase(string.find_last_not_of(std::string(flag.data(),flag.size())) + 1);
        return string;
    }

    inline bool StringCompareCase(const Magic::StringView& dest,const Magic::StringView& src){
        if(dest.size() != src.size()){
            return false;
        }
        return dest.compare(src) == 0;
    }

    inline bool StringCompareNoCase(const Magic::StringView& dest,const Magic::StringView& src){
        if(dest.size() != src.size()){
            return false;
        }
        return std::lexicographical_compare(dest.begin(),dest.end(),src.begin(),src.end(),
            [](std::string::value_type lc,std::string::value_type rc) {
                return std::tolower(lc) < std::tolower(rc);
            }
        ) == false;
    }

    inline std::vector<Magic::StringView> Split(const Magic::StringView& str,const Magic::StringView& delim){
        std::size_t previous = 0;
        std::size_t current = str.find(delim);
        std::vector<Magic::StringView> elems;
        while(current != Magic::StringView::npos){
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

    inline Magic::StringView SubString(const Magic::StringView& str,uint64_t index,const Magic::StringView& delim){
        uint64_t pos = str.find(delim,index);
        if(pos == std::string::npos)
            return str.substr(index,str.size() - index);;
        return str.substr(index,pos - index);
    }

    inline std::string TimeToString(std::time_t ts,const Magic::StringView& format = "%Y-%m-%d %H:%M:%S"){
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

    inline std::string Replace(const Magic::StringView& string,const Magic::StringView& oldStr,const Magic::StringView& newStr){
        std::string oldString(oldStr.data(),oldStr.size());
        std::string newString = std::string(string.data(),string.size());
        for(std::string::size_type pos = 0;pos != std::string::npos;pos += newStr.length()){
            if((pos = newString.find(oldString,pos)) != std::string::npos){
                newString.replace(pos,oldStr.size(),newStr.data(),newStr.size());
            }else{
                return newString;
            }
        }
        return newString;
    }

    class CaseResponsiveLess{
    public:
        bool operator()(const std::string& lhs,const std::string& rhs) const{
            return std::lexicographical_compare(lhs.begin(),lhs.end(),rhs.begin(),rhs.end(),
                [](std::string::value_type lc,std::string::value_type rc) {
                    return lc < rc;
                }
            );
        }

        bool operator()(const Magic::StringView& lhs,const Magic::StringView& rhs) const{
            return std::lexicographical_compare(lhs.begin(),lhs.end(),rhs.begin(),rhs.end(),
                [](Magic::StringView::value_type lc,Magic::StringView::value_type rc) {
                    return lc < rc;
                }
            );
        }
    };

    class CaseInsensitiveLess{
    public:
        bool operator()(const std::string& lhs,const std::string& rhs) const{
            return std::lexicographical_compare(lhs.begin(),lhs.end(),rhs.begin(),rhs.end(),
                [](std::string::value_type lc,std::string::value_type rc) {
                    return std::tolower(lc) < std::tolower(rc);
                }
            );
        }

        bool operator()(const Magic::StringView& lhs,const Magic::StringView& rhs) const{
            return std::lexicographical_compare(lhs.begin(),lhs.end(),rhs.begin(),rhs.end(),
                [](Magic::StringView::value_type lc,Magic::StringView::value_type rc) {
                    return std::tolower(lc) < std::tolower(rc);
                }
            );
        }
    };

}