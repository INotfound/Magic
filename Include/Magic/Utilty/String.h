/*
 * @Author: INotFound
 * @Date: 2020-09-12 16:23:53
 * @LastEditTime: 2020-09-30 21:29:32
 */
#pragma once
#include <ctime>
#include <string>
#include <vector>
#include <sstream>
#include <algorithm>

namespace Magic{
    static const std::string g_EmptyString;
    
    template<class T>
    std::string AsString(const T& value) {
        std::ostringstream formatStream;
        formatStream << value;
        return formatStream.str();
    }

    template<class T>
    T StringAs(const std::string& value) {
        std::stringstream formatStream;
        formatStream.clear();
        T temp;
        formatStream << value;
        formatStream >> temp;
        return temp;
    }

    template<>
    inline std::string StringAs<std::string>(const std::string& value) {
        return value;
    }

    template<>
    inline int32_t StringAs<int32_t>(const std::string& value) {
        try{
            return std::stoi(value);
        }catch(const std::exception&){
            return int32_t();
        }
    }

    template<>
    inline int64_t StringAs<int64_t>(const std::string& value) {
        try{
            return std::stol(value);
        }catch(const std::exception&){
            return int64_t();
        }
    }
    template<>
    inline uint32_t StringAs<uint32_t>(const std::string& value) {
        try{
            return static_cast<uint32_t>(std::stoul(value));
        }catch(const std::exception&){
            return uint32_t();
        }
    }
    template<>
    inline uint64_t StringAs<uint64_t>(const std::string& value) {
        try{
            return static_cast<uint64_t>(std::stoul(value));
        }catch(const std::exception&){
            return uint64_t();
        }
    }
    template<>
    inline float StringAs<float>(const std::string& value) {
        try{
            return std::stof(value);
        }catch(const std::exception&){
            return float();
        }
    }
    template<>
    inline double StringAs<double>(const std::string& value) {
        try{
            return std::stod(value);
        }catch(const std::exception&){
            return double();
        }
    }

    template<>
    inline bool StringAs<bool>(const std::string& value) {
        bool isOk = true;
        std::string tValue(value);
        {
            auto begin = tValue.begin();
            auto end = tValue.end();
            for (; begin != end; begin++)
                *begin = std::toupper(*begin);
        }
        if (tValue == std::string("FALSE") || tValue == std::string("NO") || tValue == std::string("0"))
            isOk = false;
        return isOk;
    }

    inline std::string TimeToGMTString(std::time_t tt){
        struct tm* GMTime = gmtime(&tt);
        char buff[512]={0};
        std::strftime(buff,sizeof(buff),"%a, %d %b %Y %H:%M:%S %Z",GMTime);
        return buff;
    }

    inline std::string ToLower(const std::string& string){
        std::string newString = string;
        std::transform(string.begin(),string.end(),newString.begin(),[](char c){return std::tolower(c);});
        return newString;
    }

    inline std::string ToUpper(const std::string& string){
        std::string newString = string;
        std::transform(string.begin(),string.end(),newString.begin(),[](char c){return std::toupper(c);});
        return newString;
    }

    inline std::string Trim(std::string str,const std::string flag = " ") {
        if (str.empty())
            return str;
        str.erase(0, str.find_first_not_of(flag));
        str.erase(str.find_last_not_of(flag) + 1);
        return str;
    }

    int32_t StringCompareNoCase(const std::string& dest,const std::string& src);

    inline std::vector<std::string> Split(const std::string& str,const std::string& delim) {
        std::size_t previous = 0;
        std::size_t current = str.find(delim);
        std::vector<std::string> elems;
        while (current != std::string::npos) {
            if (current > previous) {
                elems.push_back(str.substr(previous, current - previous));
            }
            previous = current + 1;
            current = str.find(delim, previous);
        }
        if (previous != str.size()) {
            elems.push_back(str.substr(previous));
        }
        return elems;
    }

    inline std::string SubString(const std::string& str, uint64_t index,const std::string& delim){
        uint64_t pos = str.find(delim, index);
        if (pos == std::string::npos)
            return str.substr(index, str.size() - index);;
        return str.substr(index, pos - index);
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
}