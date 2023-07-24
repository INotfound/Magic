/*
 ******************************************************************************
 * @Author         : INotfound
 * @File           : String.hpp
 * @Date           : 2023-07-03 18:30
 ******************************************************************************
 */
#pragma once

#include <ctime>
#include <array>
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

    template<typename T,typename = typename std::enable_if<std::is_same<T,std::string>::value || std::is_same<T,StringView>::value || std::is_integral<T>::value || std::is_floating_point<T>::value>::type>
    inline T StringAs(const StringView& value){
        T newValue = {};
        std::istringstream stringStream(std::string(value.data(),value.size()));
        stringStream >> std::dec >> newValue;
        if(stringStream.fail()){
            throw Failure("Invalid Argument!");
        }
        return newValue;
    }

    template<>
    inline bool StringAs<bool>(const StringView& value){
        bool isOk = true;
        std::string tValue;
        std::transform(value.begin(),value.end(),std::back_inserter(tValue),::toupper);
        if(tValue == "FALSE" || tValue == "NO" || tValue == "0")
            isOk = false;
        return isOk;
    }

    template<>
    inline float StringAs<float>(const StringView& value){
        float newValue = {};
        std::istringstream stringStream(std::string(value.data(),value.size()));
        stringStream >> newValue;
        if(stringStream.fail()){
            throw Failure("Invalid Argument!");
        }
        return newValue;
    }

    template<>
    inline double StringAs<double>(const StringView& value){
        double newValue = {};
        std::istringstream stringStream(std::string(value.data(),value.size()));
        stringStream >> newValue;
        if(stringStream.fail()){
            throw Failure("Invalid Argument!");
        }
        return newValue;
    }

    template<>
    inline StringView StringAs<StringView>(const StringView& value){
        return value;
    }

    template<>
    inline std::string StringAs<std::string>(const StringView& value){
        return std::string(value.data(),value.size());
    }

    template<typename T,typename = typename std::enable_if<std::is_same<T,std::string>::value || std::is_same<T,StringView>::value || std::is_integral<T>::value>::type>
    inline T StringAs(const StringView& value,uint8_t base){
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

    template<>
    inline int8_t StringAs<int8_t>(const StringView& value,uint8_t base){
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
    inline StringView StringAs<StringView>(const StringView& value,uint8_t){
        return value;
    }

    template<>
    inline std::string StringAs<std::string>(const StringView& value,uint8_t){
        return std::string(value.data(),value.size());
    }

    template<>
    inline uint8_t StringAs<uint8_t>(const StringView& value,uint8_t base){
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

    template<typename... Strings>
    inline std::string StringCat(Strings... strings){
        std::string result;
        std::array<StringView,sizeof...(Strings)> array = {strings...};
        result.resize(std::accumulate(array.cbegin(),array.cend(),0,[](uint64_t acc,const StringView& sv){
            return acc + sv.size();
        }));
        std::accumulate(array.cbegin(),array.cend(),result.begin(),[](const std::string::iterator& dest,const StringView& src){
            return std::copy(src.cbegin(),src.cend(),dest);
        });
        return result;
    }

    inline std::string ToLower(const StringView& string){
        std::string newString;
        std::transform(string.begin(),string.end(),std::back_inserter(newString),::tolower);
        return newString;
    }

    inline std::string ToUpper(const StringView& string){
        std::string newString;
        std::transform(string.begin(),string.end(),std::back_inserter(newString),::toupper);
        return newString;
    }

    inline std::string ToHexString(const StringView& string){
        size_t i,j;
        std::string result;
        result.resize(string.size() * 2);
        const auto* buffer = reinterpret_cast<const uint8_t*>(string.data());
        for(i = j = 0;i < string.size();i++){
            char c;
            c = (buffer[i] >> 4) & 0xF;
            c = (c > 9) ? c + 'a' - 10 : c + '0';
            result[j++] = c;
            c = (buffer[i] & 0xF);
            c = (c > 9) ? c + 'a' - 10 : c + '0';
            result[j++] = c;
        }
        return result;
    }

    inline std::string Trim(const StringView& str,const StringView& flag = " "){
        std::string string(str.data(),str.size());
        if(str.empty())
            return g_EmptyString;
        string.erase(0,string.find_first_not_of(std::string(flag.data(),flag.size())));
        string.erase(string.find_last_not_of(std::string(flag.data(),flag.size())) + 1);
        return string;
    }

    inline bool StringCompareCase(const StringView& dest,const StringView& src){
        if(dest.size() != src.size()){
            return false;
        }
        return dest.compare(src) == 0;
    }

    inline bool StringCompareNoCase(const StringView& dest,const StringView& src){
        if(dest.size() != src.size()){
            return false;
        }
        return std::lexicographical_compare(dest.begin(),dest.end(),src.begin(),src.end(),
            [](std::string::value_type lc,std::string::value_type rc) {
                return std::tolower(lc) < std::tolower(rc);
            }
        ) == false;
    }

    inline std::vector<StringView> Split(const StringView& str,const StringView& delim){
        std::size_t previous = 0;
        std::size_t current = str.find(delim);
        std::vector<StringView> elems;
        while(current != StringView::npos){
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

    inline StringView SubString(const StringView& str,uint64_t index,const StringView& delim){
        uint64_t pos = str.find(delim,index);
        if(pos == std::string::npos)
            return str.substr(index,str.size() - index);;
        return str.substr(index,pos - index);
    }

    inline std::string TimeToString(std::time_t ts,const StringView& format = "%Y-%m-%d %H:%M:%S"){
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

    inline std::string Replace(const StringView& string,const StringView& oldStr,const StringView& newStr){
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
        bool operator()(const StringView& lhs,const StringView& rhs) const{
            return std::lexicographical_compare(lhs.begin(),lhs.end(),rhs.begin(),rhs.end(),
                [](StringView::value_type lc,StringView::value_type rc) {
                    return lc < rc;
                }
            );
        }

        bool operator()(const std::string& lhs,const std::string& rhs) const{
            return std::lexicographical_compare(lhs.begin(),lhs.end(),rhs.begin(),rhs.end(),
                [](std::string::value_type lc,std::string::value_type rc) {
                    return lc < rc;
                }
            );
        }
    };

    class CaseInsensitiveLess{
    public:
        bool operator()(const StringView& lhs,const StringView& rhs) const{
            return std::lexicographical_compare(lhs.begin(),lhs.end(),rhs.begin(),rhs.end(),
                [](StringView::value_type lc,StringView::value_type rc) {
                    return std::tolower(lc) < std::tolower(rc);
                }
            );
        }

        bool operator()(const std::string& lhs,const std::string& rhs) const{
            return std::lexicographical_compare(lhs.begin(),lhs.end(),rhs.begin(),rhs.end(),
                [](std::string::value_type lc,std::string::value_type rc) {
                    return std::tolower(lc) < std::tolower(rc);
                }
            );
        }
    };
}