#include "Socket.h"
#include "TimingWheel.h"
namespace Magic{
    class SocketTimeOutTask :public ITaskNode{
    public:
        SocketTimeOutTask(const Share<Socket>& socket);
        void notify() override;
    private:
        Share<Socket> m_Socket;
    };
    SocketTimeOutTask::SocketTimeOutTask(const Share<Socket>& socket){
        m_Socket = socket;
    }
    void SocketTimeOutTask::notify(){
        m_Socket->runTimeOut();
    }
    Socket::~Socket(){
    }
    Socket::Socket(uint64_t timeOutMs,asio::io_context& context)
        :m_BufferSize(4096)
        ,m_TimeOut(true)
        ,m_TimeOutMs(timeOutMs)
        ,m_Socket(new asio::ip::tcp::socket(context)){
        m_BlockBuffer.reset(new char[m_BufferSize]);
        m_ReadBuffer.reserve(m_BufferSize);
        m_WriteBuffer.reserve(m_BufferSize);
        TimingWheel::GetInstance()->run();

    }
    const Safe<asio::ip::tcp::socket>& Socket::getEntity() const{
        return m_Socket;
    }
    // void read(std::function<void(const std::error_code& ec,const std::vector<char>& data)> callBack);
    // void read(uint64_t size,std::function<void(const std::error_code& ec,const std::vector<char>& data)> callBack);
    // void write(const std::string& data,std::function<void(const std::error_code& ec,const std::vector<char>& data)> callBack);
    // void write(const char* data,uint64_t size,std::function<void(const std::error_code& ec,const std::vector<char>& data)> callBack);
    void Socket::enableTimeOut(){
        Safe<ITaskNode> node(new SocketTimeOutTask(this->shared_from_this()));
        TimingWheel::GetInstance()->addTask(m_TimeOutMs,node);
    }
    void Socket::updateTimeOut(){
        m_TimeOut = false;
    }
    void Socket::doRead(std::function<void(const std::error_code& ec)> callBack){
        auto self = this->shared_from_this();
        asio::async_read(*m_Socket
            ,asio::buffer(m_BlockBuffer.get(),m_BufferSize)
            ,asio::transfer_at_least(1)
            ,[this,self,callBack](const asio::error_code &err, std::size_t length){
            if(!err){
                this->updateTimeOut();
                m_ReadBuffer.insert(m_ReadBuffer.begin() + m_ReadBuffer.size()
                    ,m_BlockBuffer.get()
                    ,m_BlockBuffer.get() + length);
            }
            callBack(err);
        });
    }
    void Socket::doWrite(std::function<void(const std::error_code& ec)> callBack){
        auto self = this->shared_from_this();
        asio::async_write(*m_Socket
            ,asio::const_buffer(m_WriteBuffer.data(),m_WriteBuffer.size())
            ,[this,self,callBack](const asio::error_code &err, std::size_t length){
            this->updateTimeOut();
            callBack(err);
        });
    }
    void Socket::doRead(uint64_t size,std::function<void(const std::error_code& ec)> callBack){
        auto self = this->shared_from_this();
        asio::async_read(*m_Socket
            ,asio::buffer(m_BlockBuffer.get(),m_BufferSize)
            ,asio::transfer_exactly(size)
            ,[this,self,callBack](const asio::error_code &err, std::size_t length){
            if(!err){
                this->updateTimeOut();
                m_ReadBuffer.insert(m_ReadBuffer.begin() + m_ReadBuffer.size()
                    ,m_BlockBuffer.get()
                    ,m_BlockBuffer.get() + length);
            }
            callBack(err);
        });
    }
    void Socket::runTimeOut(){
        if(m_TimeOut == true){
            m_Socket->close();
        }else{
            m_TimeOut = true;
            Safe<ITaskNode> node(new SocketTimeOutTask(this->shared_from_this()));
            TimingWheel::GetInstance()->addTask(m_TimeOutMs,node);
        }
    }
}