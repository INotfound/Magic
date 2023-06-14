#pragma once
#if __cplusplus >= 201703L
#include <string_view>
#else

#include <string>
#include <cstring>
#include <ostream>
#include <exception>

namespace std{
    class string_view{
    public:
        using value_type = char;
        using size_type = std::size_t;
        using const_pointer = const char*;
        using const_iterator = const char*;
        using const_reference = const char&;
        static constexpr size_type npos = size_type(-1);
    public:
        constexpr string_view() noexcept :m_Data(nullptr),m_Size(0){}

        string_view(const std::string& str) noexcept :m_Data(str.data()),m_Size(str.size()){}

        constexpr string_view(const char* data,size_type size) noexcept :m_Data(data),m_Size(size){}

        string_view(const char* data) noexcept :m_Data(data),m_Size(std::char_traits<value_type>::length(data)){}

        constexpr string_view(const string_view&) noexcept = default;

        string_view& operator=(const string_view&) noexcept = default;

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

        constexpr bool empty() const noexcept{
            return m_Size == 0;
        }

        constexpr size_type size() const noexcept{
            return m_Size;
        }

        constexpr size_type length() const noexcept{
            return m_Size;
        }

        constexpr const_pointer data() const noexcept{
            return m_Data;
        }
        constexpr const_reference at(size_type pos) const{
            return pos >= m_Size ? throw std::out_of_range("string_view::at"),m_Data[0] : m_Data[pos];
        }

    #if __cplusplus >= 201402L
        constexpr
    #endif
        string_view substr(size_type pos,size_type n = npos) const{
            if(pos > m_Size){
                throw std::out_of_range("string_view::substr");
            }
            return string_view(data() + pos,std::min(m_Size - pos,n));
        }

        constexpr const_reference operator[](size_type pos) const noexcept{
            return m_Data[pos];
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

        size_type find(string_view sv,size_type pos = 0) const noexcept{
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

        size_type rfind(string_view sv,size_type pos = npos) const noexcept{
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

        size_type rfind(value_type c,size_type pos = npos) const noexcept{
            return rfind(string_view(&c,1),pos);
        }

        int compare(string_view x) const noexcept{
            const int cmp = std::char_traits<value_type>::compare(m_Data,x.data(),std::min(m_Size,x.size()));
            return cmp != 0 ? cmp : (m_Size == x.size() ? 0 : (m_Size < x.size() ? -1 : 1));
        }
    private:
        const_pointer m_Data;
        size_type m_Size;
    };

    inline ostream& operator<<(ostream& os,const string_view& sv){
        os.write(sv.data(),sv.size());
        return os;
    }

    inline bool operator==(const string_view& x,const string_view& y) noexcept{
        if(x.size() != y.size()){
            return false;
        }
        return x.compare(y) == 0;
    }

    inline bool operator!=(const string_view& x,const string_view& y) noexcept{
        if(x.size() != y.size()){
            return false;
        }
        return x.compare(y) != 0;
    }
}

#endif

