#pragma once 

#include <regex>
#include <vector>
#include <istream>
#include <ostream>
#include <unordered_map>

#include "Core.h"
#include "Thread.h"
#include "asio.hpp"

namespace Magic{


    typedef struct Request{
        std::string m_Path{};
        std::string m_Method{};
        std::string m_HttpVer{};
        std::smatch m_PathMatch{};
        MagicPtr<std::istream> m_Content{};
        std::unordered_map<std::string, std::string> header;
    };

    typedef std::map<std::string, std::unordered_map<std::string,
    std::function<void(std::ostream&, Request&)>>> Resource;

    template <typename SocketType>
    class IServer{
        public:
            ~IServer();
            IServer(uint32_t port,uint32_t threads =1)
                :m_Endpoint(asio::ip::tcp::v4(),port)
                ,m_Acceptor(m_IoService,m_Endpoint)
                ,m_ThreadCount(threads){
            }

            void start(){
                for(auto iter=m_Resource->begin(); iter != m_Resource->end(); iter++){
                    m_Resources.push_back(iter);
                }
                for(auto iter=m_DefualtResource->begin(); iter != m_DefualtResource->end(); iter++){
                    m_Resources.push_back(iter);
                }
                accept();

                for(uint32_t i{1};i < m_ThreadCount;i++){
                    m_Threads.emplace_back(MagicPtr<Magic::Thread>(new Magic::Thread("",[this](){
                        m_IoService.run();
                    })));
                }
                m_IoService.run();
                for(auto &thrs : m_Threads){
                    thrs.join();
                }
            }
        protected:
            virtual void accept(){};
            void processRequestAndResponse(MagicPtr<SocketType> &socket) const{
                auto readBuffer = std::make_shared<asio::streambuf>();
                asio::async_read_until(*socket,*readBuffer,"\r\n\r\n",
                [this,socket,readBuffr](std::error_code ec, std::size_t length){
                    if(!ec){
                        uint32_t total = readBuffer->size();
                        std::istream stream(readBuffer.get());

                        auto request = std::make_shared<Request>();
                        *request = parseRequest(stream);
                        uint32_t  additionalBytes = total - length;
                        if(request->header.count("Content-Length") > 0){
                            asio::async_read(*socket,*readBuffer,
                                asio::tranfer_exactly(std::stoull(request->header["Content-Length"]) - additionalBytes),
                                [this,socket,readBuffer,request](){
                                    if(!ec){
                                        request->m_Content->reset(new std::istream(readBuffer.get()));
                                        response(socket,request);
                                    }
                            });
                        }else{
                            response(socket,request);
                        }
                    }
                });
            }
            Request parseRequest(std::istream& stream) const{
                Request requset{};
                std::regex reg("^([^ ]*) ([^ ]*) HTTP/([^ ]*)$");
                std::smatch subMatch{};
                std::string line{};
                getline(stream,line);
                line.pop_back();
                if(std::regex_match(line,subMatch,reg)){
                    requset.m_Method    = subMatch[1];
                    requset.m_Path      = subMatch[2];
                    requset.m_HttpVer   = subMatch[3];
                }
                bool matched{false};
                reg = "^([^:]*): ?(.*)$";
                do{
                    getline(stream,line);
                    line.pop_back();
                    matched = std::regex_match(line,subMatch,reg);
                    if(matched){
                        request.header[subMatch[1]] = subMatch[2];
                    }
                }while(matched == true)
                return request;
            }
            void response(MagicPtr<SocketType> &socket,Resquest &request) const{
                for(auto &res : m_Resources){
                    std::regex reg(res->first);
                    std::smatch match;
                    if(std::regex_match(request->path,match,reg)){
                        request->m_PathMatch = move (match);
                        
                        auto writeBuffer = std::make_shared<asio::streambuf>();
                        std::ostream response(wirteBuffer.get());
                        match->sercond[request->method](response,*request);
                        asio::async_write(*socket,*writeBuffer,
                        [this,socket,request,writeBuffer](const std::error_code &ec,size_t length){
                            if(!ec && stof(request->m_HttpVer) > 1.05){
                                this->processRequestAndResponse(socket);
                            }
                        });
                        return;
                    }
                }
            }
        public:
            MagicPtr<Resource> m_Resource;
            MagicPtr<Resource> m_DefualtResource;
        protected:
            std::vector<Resource> m_Resources;
        private:
            uint64_t m_ThreadCount{};
            asio::io_service m_IoService{};
            asio::ip::tcp::endpoint m_Endpoint{};
            asio::ip::tcp::acceptor m_Acceptor{};
            std::vector<MagicPtr<Magic::Thread>> m_Threads{};
    };

}