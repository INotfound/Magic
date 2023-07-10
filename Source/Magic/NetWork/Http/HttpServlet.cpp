/*
 ******************************************************************************
 * @Author         : INotfound
 * @File           : HttpServlet.cpp
 * @Date           : 2023-07-03 18:33
 ******************************************************************************
 */
#include <regex>
#include "Magic/Utilty/Trace.hpp"
#include "Magic/Utilty/Logger.hpp"
#include "Magic/NetWork/Http/HttpServlet.hpp"

namespace Magic{
namespace NetWork{
namespace Http{

    std::string GenerateHtml(const Magic::StringView& status,const Magic::StringView& title,const Magic::StringView& message){
        std::stringstream html;
        html << "<!DOCTYPE html><html lang=\"en\"><head><title>"
             << status
             << "</title></head><body><center><h1>"
             << title
             << "</h1><h3>"
             << message
             << "</h3></center><hr><center>Magic/2.0.0</center></body></html>";
        return html.str();
    }

    IHttpServlet::~IHttpServlet() = default;

    void HttpServletDispatch::handle(const Safe<HttpSocket>& httpSocket){
        std::lock_guard<std::mutex> locker(m_Mutex);
        Magic::StringView httpPath = httpSocket->getRequest()->getPath();
        try{
            auto exactlyIter = m_NormalRoutes.find(std::string(httpPath.data(),httpPath.size()));
            auto exactlyEnd = m_NormalRoutes.end();
            if(exactlyIter != exactlyEnd){
            #ifdef TRACE
                TraceTimer traceTimer(httpPath);
            #endif
                /// Global Before
                for(const auto& func : m_AspectBefores){
                    if(!func(httpSocket))
                        return;
                }
                const auto& aspectAfters = std::get<2>(exactlyIter->second);
                const auto& aspectBefores = std::get<1>(exactlyIter->second);
                /// Before
                for(const auto& func : aspectBefores){
                    if(!func(httpSocket))
                        return;
                }
                /// Handle
                std::get<0>(exactlyIter->second)(httpSocket);
                /// After
                for(const auto& func : aspectAfters){
                    if(!func(httpSocket))
                        return;
                }
                /// Global After
                for(const auto& func : m_AspectAfters){
                    if(!func(httpSocket))
                        return;
                }
                return;
            }

            std::regex reg;
            auto matchEnd = m_MatchRoutes.end();
            auto matchIter = m_MatchRoutes.begin();
            for(;matchIter != matchEnd;matchIter++){
                reg.assign(matchIter->first);
                if(std::regex_match(httpPath.begin(),httpPath.end(),reg)){
                #ifdef TRACE
                    TraceTimer traceTimer(httpPath);
                #endif
                    /// Global Before
                    for(const auto& func : m_AspectBefores){
                        if(!func(httpSocket))
                            return;
                    }
                    const auto& aspectAfters = std::get<2>(matchIter->second);
                    const auto& aspectBefores = std::get<1>(matchIter->second);
                    /// Before
                    for(const auto& func : aspectBefores){
                        if(!func(httpSocket))
                            return;
                    }
                    /// Handle
                    std::get<0>(matchIter->second)(httpSocket);
                    /// After
                    for(const auto& func : aspectAfters){
                        if(!func(httpSocket))
                            return;
                    }
                    /// Global After
                    for(const auto& func : m_AspectAfters){
                        if(!func(httpSocket))
                            return;
                    }
                    return;
                }
            }
        }catch(const std::exception& ec){
            MAGIC_WARN() << ec.what();
            const auto& response = httpSocket->getResponse();
            response->setStatus(HttpStatus::InternalServerError)
                    ->setBody(GenerateHtml(HttpStatusToString(HttpStatus::InternalServerError),"Exception",ec.what()));
            httpSocket << response;
            return;
        }
        const auto& response = httpSocket->getResponse();
        response->setStatus(HttpStatus::NotFound);
        httpSocket << response;
    }

    void HttpServletDispatch::setHttpServlet(const Safe<IHttpServlet>& servlet){
        std::lock_guard<std::mutex> locker(m_Mutex);
        servlet->m_ServletDispatch = this->shared_from_this();
    }
}
}
}
