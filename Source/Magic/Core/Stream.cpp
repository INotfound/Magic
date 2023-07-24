/*
 ******************************************************************************
 * @Author         : INotfound
 * @File           : Stream.cpp
 * @Date           : 2023-07-19 12:33
 ******************************************************************************
 */
#include "Magic/Core/Stream.hpp"
#include "Magic/Core/StringView.hpp"

namespace Magic{
    IStream::~IStream() = default;

    DataStream::DataStream()
        :m_Position(0){
    }

    DataStream::DataStream(const StringView& data)
        :m_Position(0)
        ,m_Buffer(data.cbegin(),data.cend()){
    }

    void DataStream::resize(uint64_t size){
        m_Buffer.resize(size);
        m_Position = 0;
    }

    void DataStream::seek(uint64_t pos){
        m_Position = pos;
    }

    bool DataStream::eof() const noexcept{
        return m_Position == m_Buffer.size();
    }

    IStream::BufferView DataStream::read(){
        auto bufferView = IStream::BufferView(m_Buffer.data()+m_Position,m_Buffer.size()-m_Position);
        m_Position = m_Buffer.size();
        return bufferView;
    }

    IStream::BufferType::pointer DataStream::data() noexcept{
        return m_Buffer.data();
    }

    uint64_t DataStream::size() const noexcept{
        return m_Buffer.size();
    }

    void DataStream::write(const IStream::BufferView& data){
        m_Buffer.insert(m_Buffer.end(),data.cbegin(),data.cend());
    }

    FileStream::FileStream(const StringView& path)
        :m_Position(0)
        ,m_FileSize(0)
        ,m_BufferSize(65536)
        ,m_FilePath(path.begin(),path.end())
        ,m_Buffer(nullptr,[](char* pointer){
            if(pointer)
                delete[] pointer;
        })
        ,m_File(nullptr,[](std::FILE* pointer){
            if(pointer){
                clearerr(pointer);
                fclose(pointer);
            }
        }){
    }

    bool FileStream::open(OpenMode mode){
        StringView openType;
        switch(mode){
            case OpenMode::Read:
                openType = "rb";
                break;
            case OpenMode::Write:
                openType = "wb";
                break;
            case OpenMode::Append:
                openType = "ab";
                break;
            case OpenMode::ReadWrite:
                openType = "rb+";
                break;
        }
        m_File.reset(std::fopen(m_FilePath.data(),openType.data()));
        if(!m_File){
            return false;
        }
        const auto current = std::ftell(m_File.get());
        if(current < 0){
            return false;
        }
        if(std::fseek(m_File.get(),0,SEEK_END) != 0){
            return false;
        }

        const auto totalSize = std::ftell(m_File.get());
        if(totalSize < 0){
            return false;
        }
        if(std::fseek(m_File.get(),current,SEEK_SET) != 0){
            return false;
        }
        /// Min 64K Max 1M
        m_BufferSize = totalSize > 65536 ? (totalSize > 262144 ? (totalSize > 524288 ? 1048576 : 524288) : 262144) : 65536;
        m_Buffer.reset(new char[m_BufferSize]);
        m_FileSize = totalSize;
        return true;
    }

    void FileStream::seek(uint64_t pos){
        if(!m_File){
            return;
        }
        if(std::fseek(m_File.get(),pos,SEEK_SET) == 0){
            m_Position = pos;
        }
    }

    bool FileStream::eof() const noexcept{
        return m_Position == m_FileSize;
    }

    IStream::BufferView FileStream::read(){
        if(!m_File || !m_Buffer || std::feof(m_File.get()) != 0){
            m_Position = m_FileSize;
            return IStream::BufferView();
        }
        auto size = std::fread(m_Buffer.get(),sizeof(char),m_BufferSize,m_File.get());
        m_Position += size;
        return IStream::BufferView(m_Buffer.get(),size);
    }

    uint64_t FileStream::size() const noexcept{
        return m_FileSize;
    }

    void FileStream::write(const IStream::BufferView& data){
        if(!m_File || !m_Buffer || std::ferror(m_File.get()) != 0){
            return;
        }
        auto size = std::fwrite(data.data(),sizeof(char),data.size(),m_File.get());
        std::fflush(m_File.get());
        m_FileSize += size;
    }
}

