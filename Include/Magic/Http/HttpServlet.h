#pragma once
#include <unordered_map>

#include "Core.h"
#include "Mutex.h"
#include "Http/Http.h"
namespace Magic{
namespace Http{
    class HttpServlet{
    public:
        explicit HttpServlet(const std::string& name);
        const std::string getName() const;
        virtual bool handle (Safe<HttpRequest>& request,Safe<HttpResponse>& response) =0;
    private:
        std::string m_Name;
    };

    class HttpServletDispatch :HttpServlet{
        typedef RWMutex::WriteLock RWMutexWriteLock;
    public:
        HttpServletDispatch();
        void setDeafultServlet(Safe<HttpServlet>& servlet);
        void addHttpServlet(const std::string& name,Safe<HttpServlet>& servlet);
        void addGlobHttpServlet(const std::string& name,Safe<HttpServlet>& servlet);
        bool handle(Safe<HttpRequest>& request,Safe<HttpResponse>& response) override;
    private:
        Safe<HttpServlet>& getMatchedServlet(const std::string& name);
    private:
        RWMutex m_Mutex;
        Safe<HttpServlet> m_DeafultServlet;
        std::unordered_map<std::string,Safe<HttpServlet>> m_Servlets;
        std::unordered_map<std::string,Safe<HttpServlet>> m_GlobServlets;
    };

}
}