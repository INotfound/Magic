/*
 ******************************************************************************
 * @Author         : INotfound
 * @File           : StringView.hpp
 * @Date           : 2023-07-03 18:26
 ******************************************************************************
 */
#pragma once
#include <string>
#include <numeric>
#if __cplusplus >= 201703L
#include <string_view>
namespace Magic{using StringView = std::string_view;}
#else
#include <cstring>
#include <ostream>
#include <exception>

namespace Magic{
    class StringView{
    public:
        using value_type = char;
        using size_type = std::size_t;
        using const_pointer = const char*;
        using const_iterator = const char*;
        using const_reference = const char&;
        static constexpr size_type npos = size_type(-1);
    public:
        constexpr StringView() noexcept
            :m_Size(0),m_Data(nullptr){}


        StringView(const char* data) noexcept
            :m_Size(std::char_traits<value_type>::length(data)),m_Data(data){}

        StringView(const std::string& str) noexcept
            :m_Size(str.size()),m_Data(str.data()){}


        constexpr StringView(const StringView&) noexcept = default;

        StringView& operator=(const StringView&) noexcept = default;

        constexpr StringView(const char* data,size_type size) noexcept
            :m_Size(size),m_Data(data){}

        constexpr bool empty() const noexcept{
            return m_Size == 0;
        }

        constexpr size_type size() const noexcept{
            return m_Size;
        }

        constexpr size_type length() const noexcept{
            return m_Size;
        }

        int32_t compare(StringView x) const noexcept{
            const int32_t cmp = std::char_traits<value_type>::compare(m_Data,x.data(),std::min(m_Size,x.size()));
            return cmp != 0 ? cmp : (m_Size == x.size() ? 0 : (m_Size < x.size() ? -1 : 1));
        }

        constexpr const_pointer data() const noexcept{
            return m_Data;
        }

        constexpr const_iterator end() const noexcept{
            return m_Data + m_Size;
        }

        constexpr const_iterator cend() const noexcept{
            return m_Data + m_Size;
        }

        constexpr const_iterator begin() const noexcept{
            return m_Data;
        }

        constexpr const_iterator cbegin() const noexcept{
            return m_Data;
        }

        constexpr const_reference at(size_type pos) const{
            return pos >= m_Size ? throw std::out_of_range("StringView::at"),m_Data[0] : m_Data[pos];
        }

    #if __cplusplus >= 201402L
        constexpr
    #endif
        StringView substr(size_type pos,size_type n = npos) const{
            if(pos > m_Size){
                throw std::out_of_range("StringView::substr");
            }
            return StringView(data() + pos,std::min(m_Size - pos,n));
        }

        size_type find(value_type c,size_type pos = 0) const noexcept{
            if (pos > size()){
                return npos;
            }
            const_pointer pointer = std::char_traits<value_type>::find(m_Data + pos,m_Size - pos,c);
            if(pointer){
                return pointer - m_Data;
            }
            return npos;
        }

        size_type find(StringView sv,size_type pos = 0) const noexcept{
            if (pos > m_Size){
                return npos;
            }
            if (sv.empty()){
                return pos;
            }
            if (sv.size() > m_Size - pos){
                return npos;
            }
            const_pointer cur = m_Data + pos;
            const_pointer last = cend() - sv.size() + 1;
            for (; cur != last; cur++) {
                cur = std::char_traits<value_type>::find(cur,last - cur,sv[0]);
                if (!cur)
                    return npos;
                if (std::char_traits<value_type>::compare(cur,sv.cbegin(),sv.size()) == 0)
                    return cur - m_Data;
            }
            return npos;
        }

        size_type rfind(value_type c,size_type pos = npos) const noexcept{
            return rfind(StringView(&c,1),pos);
        }

        size_type rfind(StringView sv,size_type pos = npos) const noexcept{
            if (m_Size < sv.size()){
                return npos;
            }
            if (pos > m_Size - sv.size()){
                pos = m_Size - sv.size();
            }
            if (sv.size() == 0u){
                return pos;
            }

            for(const_pointer cur = m_Data + pos;;cur--){
                if (std::char_traits<value_type>::compare(cur,sv.data(),sv.size()) == 0)
                    return cur - m_Data;
                if (cur == m_Data)
                    return npos;
            };
        }

        constexpr const_reference operator[](size_type pos) const noexcept{
            return m_Data[pos];
        }
    private:
        size_type m_Size;
        const_pointer m_Data;
    };

    inline std::ostream& operator<<(std::ostream& os,const StringView& sv){
        os.write(sv.data(),sv.size());
        return os;
    }

    inline bool operator==(const StringView& x,const StringView& y) noexcept{
        if(x.size() != y.size()){
            return false;
        }
        return x.compare(y) == 0;
    }

    inline bool operator!=(const StringView& x,const StringView& y) noexcept{
        if(x.size() != y.size()){
            return false;
        }
        return x.compare(y) != 0;
    }
}

#endif