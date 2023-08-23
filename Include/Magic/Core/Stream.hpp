/*
 ******************************************************************************
 * @Author         : INotfound
 * @File           : Stream.hpp
 * @Date           : 2023-07-19 10:30
 ******************************************************************************
 */
#pragma once

#include "Magic/Core/Core.hpp"
#include "Magic/Core/StringView.hpp"

namespace Magic{
    class IStream :public Noncopyable{
    public:
        using BufferView = StringView;
        using BufferType = std::vector<char>;
    public:
        virtual ~IStream();
        virtual BufferView read() = 0;
        virtual void seek(uint64_t pos) = 0;
        virtual bool eof() const noexcept = 0;
        virtual uint64_t size() const noexcept = 0;
        virtual void write(const BufferView& data) = 0;
    };

    class DataStream :public IStream{
    public:
        DataStream();
        explicit DataStream(const StringView& data);

        void resize(uint64_t size);
        BufferView read() override;
        void seek(uint64_t pos) override;
        bool eof() const noexcept override;
        BufferType::pointer data() noexcept;
        uint64_t size() const noexcept override;
        void write(const BufferView& data) override;
    private:
        uint64_t m_Position;
        BufferType m_Buffer;
    };

    class FileStream :public IStream{
    public:
        enum class OpenMode :uint32_t{
            Read        = 1,
            Write       = 10,
            Append      = 100,
            ReadWrite   = 1000
        };
    public:
        explicit FileStream(const StringView& path);

        bool remove();
        bool open(OpenMode mode);
        StringView getPath() const;
        BufferView read() override;
        void seek(uint64_t pos) override;
        bool eof() const noexcept override;
        uint64_t size() const noexcept override;
        void write(const BufferView& data) override;
        bool copy(const Magic::StringView& newPath);
        bool move(const Magic::StringView& newPath);
    private:
        uint64_t m_Position;
        uint64_t m_FileSize;
        uint64_t m_BufferSize;
        std::string m_FilePath;
        std::unique_ptr<char,void(*)(char*)> m_Buffer;
        std::unique_ptr<std::FILE,void(*)(std::FILE*)> m_File;
    };
}
