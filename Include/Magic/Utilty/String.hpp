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

    /**
     * @brief 字符串拼接函数
     * @param strings 可变参字符串
     * @return 返回拼接好的字符串
     */
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

    /**
     * @brief 转换小写字符串
     * @param string 需要转换的字符串
     * @return 转换好的字符串
     */
    inline std::string ToLower(const StringView& string){
        std::string newString;
        std::transform(string.begin(),string.end(),std::back_inserter(newString),::tolower);
        return newString;
    }

    /**
     * @brief 转换大写字符串
     * @param string 需要转换的字符串
     * @return 转换好的字符串
     */
    inline std::string ToUpper(const StringView& string){
        std::string newString;
        std::transform(string.begin(),string.end(),std::back_inserter(newString),::toupper);
        return newString;
    }

    /**
     * @brief 转换Hex字符串
     * @param string 需要转换的字符串
     * @return 转换好的字符串
     */
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

    /**
     * @brief 修整掉字符串的前后 flag
     * @param stringView 需要修正的字符串
     * @param flag 修整掉的字符串
     * @return 修整好的字符串
     */
    inline std::string Trim(const StringView& stringView,const StringView& flag = " "){
        std::string string(stringView.data(),stringView.size());
        if(stringView.empty())
            return g_EmptyString;
        std::string flagString(flag.data(),flag.size());
        string.erase(0,string.find_first_not_of(flagString));
        string.erase(string.find_last_not_of(flagString) + 1);
        return string;
    }

    /**
     * @brief 分割字符串
     * @param str 需要分割的字符串
     * @param delim 分割符
     * @return 返回分割完成后的字符串数组
     */
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

    /**
     * @brief 截取子字符串
     * @param stringView 字符串
     * @param delim 截取字符串
     * @param index 起始位置
     * @return 返回字符串
     */
    inline StringView SubString(const StringView& stringView,const StringView& delim,uint64_t index = 0){
        uint64_t pos = stringView.find(delim,index);
        if(pos == std::string::npos)
            return stringView.substr(index,stringView.size() - index);;
        return stringView.substr(index,pos - index);
    }

    /**
     * @brief 格式化时间
     * @param ts time 需要转换的时间
     * @param format 转换格式
     * @return 转换好的时间字符串
     */
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

    /**
     * @brief 替换字符串
     * @param string 需要替换的字符串
     * @param oldStr 老字符串
     * @param newStr 新字符串
     * @return 替换好的字符串
     */
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