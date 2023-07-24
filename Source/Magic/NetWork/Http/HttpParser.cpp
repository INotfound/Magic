/*
 ******************************************************************************
 * @Author         : INotfound
 * @File           : HttpParser.cpp
 * @Date           : 2023-07-03 18:32
 ******************************************************************************
 */
#include <cstring>
#include "Magic/Utilty/String.hpp"
#include "Magic/NetWork/Http/HttpParser.hpp"
#include "Magic/NetWork/Http/Utils/http11_parser.hpp"
#include "Magic/NetWork/Http/Utils/httpclient_parser.hpp"

namespace Magic{
namespace NetWork{
namespace Http{
    void OnRequestUri(void* /*data*/,const char* /*at*/,size_t /*length*/){
    }

    void OnRequestPath(void* data,const char* at,size_t length){
        auto* parser = static_cast<HttpRequestParser*>(data);
        parser->getData()->setPath(StringView(at,length));
    }

    void OnRequestQuery(void* data,const char* at,size_t length){
        auto* parser = static_cast<HttpRequestParser*>(data);
        parser->getData()->setQuery(StringView(at,length));
    }

    void OnRequestMethod(void* data,const char* at,size_t length){
        char buffer[126] = {0};
        auto* parser = static_cast<HttpRequestParser*>(data);
        std::memcpy(buffer,at,std::min(static_cast<size_t>(125),length));
        HttpMethod method = StringToHttpMethod(StringView(buffer,length));
        if(method == HttpMethod::InvalidMethod){
            parser->setError(true);
            return;
        }
        parser->getData()->setMethod(method);
    }

    void OnRequestVersion(void* data,const char* at,size_t length){
        auto* parser = static_cast<HttpRequestParser*>(data);
        uint8_t ver = 0;
        if(std::strncmp(at,"HTTP/1.1",length) == 0){
            ver = 0x11;
        }else if(std::strncmp(at,"HTTP/1.0",length) == 0){
            ver = 0x10;
        }else{
            //TODO:
            parser->setError(true);
            return;
        }
        parser->getData()->setVersion(ver);
    }

    void OnRequestFragment(void* data,const char* at,size_t length){
        auto* parser = static_cast<HttpRequestParser*>(data);
        parser->getData()->setFragment(StringView(at,length));
    }

    void OnRequestHeaderDone(void* /*data*/,const char* /*at*/,size_t /*length*/){
        //HttpRequestParser *parser = static_cast<HttpRequestParser*>(data);
    }

    void OnRequestHttpField(void* data,const char* field,size_t flen,const char* value,size_t vlen){
        auto* parser = static_cast<HttpRequestParser*>(data);
        if(flen == 0){
            //TODO
            parser->setError(true);
            return;
        }
        parser->getData()->setHeader(StringView(field,flen),StringView(value,vlen));
    }

    HttpRequestParser::HttpRequestParser()
        :m_Error(false)
        ,m_Data(std::make_shared<HttpRequest>())
        ,m_Parser(std::make_shared<http_parser>()){

        http_parser_init(m_Parser.get());
        m_Parser->request_uri = OnRequestUri;
        m_Parser->request_path = OnRequestPath;
        m_Parser->fragment = OnRequestFragment;
        m_Parser->query_string = OnRequestQuery;
        m_Parser->http_version = OnRequestVersion;
        m_Parser->http_field = OnRequestHttpField;
        m_Parser->request_method = OnRequestMethod;
        m_Parser->header_done = OnRequestHeaderDone;
        m_Parser->data = this;
    }

    void HttpRequestParser::reset(){
        m_Error = false;
        m_Data.reset();
        m_Data = std::make_shared<HttpRequest>();
        http_parser_init(m_Parser.get());
    }

    bool HttpRequestParser::hasError(){
        return m_Error || http_parser_has_error(m_Parser.get());
    }

    bool HttpRequestParser::isFinished(){
        return http_parser_finish(m_Parser.get());
    }

    void HttpRequestParser::setError(bool val){
        m_Error = val;
    }

    uint64_t HttpRequestParser::getContentLength(){
        StringView length = m_Data->getHeader("Content-Length");
        if(length.empty()){
            m_Data->setContentLength(0);
        }else{
            try{
                m_Data->setContentLength(StringAs<uint64_t>(length,10));
            }catch(...){
                m_Data->setContentLength(0);
            }
        }
        return m_Data->getContentLength();
    }

    const Safe<HttpRequest>& HttpRequestParser::getData(){
        return m_Data;
    }

    uint32_t HttpRequestParser::execute(char* data,uint32_t length){
        size_t offset = http_parser_execute(m_Parser.get(),data,length,0);
        std::memmove(data,data + offset,length - offset);
        return offset;
    }

    void OnResponseReason(void* data,const char* at,size_t length){
        auto* parser = static_cast<HttpResponseParser*>(data);
        parser->getData()->setReason(StringView(at,length));
    }

    void OnResponseStatus(void* data,const char* at,size_t /*length*/){
        auto* parser = static_cast<HttpResponseParser*>(data);
        auto status = static_cast<HttpStatus>(atol(at));
        parser->getData()->setStatus(status);
    }

    void OnResponseVersion(void* data,const char* at,size_t length){
        auto* parser = static_cast<HttpResponseParser*>(data);
        uint8_t ver = 0;
        if(std::strncmp(at,"HTTP/1.1",length) == 0){
            ver = 0x11;
        }else if(std::strncmp(at,"HTTP/1.0",length) == 0){
            ver = 0x10;
        }else{
            //TODO:
            parser->setError(true);
            return;
        }
        parser->getData()->setVersion(ver);
    }

    void OnResponseLastChunk(void* /*data*/,const char* /*at*/,size_t /*length*/){
        //HttpResponseParser *parser = static_cast<HttpResponseParser*>(data);
    }

    void OnResponseChunkSize(void* /*data*/,const char* /*at*/,size_t /*length*/){
        //HttpResponseParser *parser = static_cast<HttpResponseParser*>(data);
    }

    void OnResponseHeaderDone(void* /*data*/,const char* /*at*/,size_t /*length*/){
    }

    void OnResponseHttpField(void* data,const char* field,size_t flen,const char* value,size_t vlen){
        auto* parser = static_cast<HttpResponseParser*>(data);
        if(flen == 0){
            //TODO
            parser->setError(true);
            return;
        }
        parser->getData()->setHeader(StringView(field,flen),StringView(value,vlen));
    }

    HttpResponseParser::HttpResponseParser()
        :m_Error(false)
        ,m_Data(std::make_shared<HttpResponse>())
        ,m_Parser(std::make_shared<httpclient_parser>()){
        httpclient_parser_init(m_Parser.get());
        m_Parser->status_code = OnResponseStatus;
        m_Parser->reason_phrase = OnResponseReason;
        m_Parser->http_version = OnResponseVersion;
        m_Parser->last_chunk = OnResponseLastChunk;
        m_Parser->chunk_size = OnResponseChunkSize;
        m_Parser->http_field = OnResponseHttpField;
        m_Parser->header_done = OnResponseHeaderDone;
        m_Parser->data = this;
    }

    void HttpResponseParser::reset(){
        m_Error = false;
        m_Data = std::make_shared<HttpResponse>();
        httpclient_parser_init(m_Parser.get());
    }

    bool HttpResponseParser::hasError(){
        return m_Error || httpclient_parser_has_error(m_Parser.get());;
    }

    bool HttpResponseParser::isFinished(){
        return httpclient_parser_finish(m_Parser.get());
    }

    void HttpResponseParser::setError(bool val){
        m_Error = val;
    }

    uint32_t HttpResponseParser::getContentLength(){
        StringView length = m_Data->getHeader("Content-Length");
        if(length.empty()){
            m_Data->setContentLength(0);
        }else{
            try{
                m_Data->setContentLength(StringAs<uint64_t>(length,10));
            }catch(...){
                m_Data->setContentLength(0);
            }
        }
        return m_Data->getContentLength();
    }

    const Safe<HttpResponse>& HttpResponseParser::getData(){
        return m_Data;
    }

    uint32_t HttpResponseParser::execute(char* data,uint32_t length){
        size_t offset = httpclient_parser_execute(m_Parser.get(),data,length,0);
        std::memmove(data,data + offset,length - offset);
        return offset;
    }
}
}
}
