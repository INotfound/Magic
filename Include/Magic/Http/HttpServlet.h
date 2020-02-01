#pragma once
#include <functional>
#include <unordered_map>

#include "Core.h"
#include "Mutex.h"
#include "Http/Http.h"
#include "Http/HttpSession.h"
namespace Magic{
namespace Http{
    class HttpServlet{
    public:
        explicit HttpServlet(const std::string& name);
        const std::string getName() const;
        virtual void handle (const std::shared_ptr<HttpSession>& session,MagicPtr<HttpRequest>& request,MagicPtr<HttpResponse>& response) =0;
    private:
        std::string m_Name{};
    };

    class HttpServletDispatch :HttpServlet{
        typedef RWMutex::WriteLock RWMutexWriteLock;
    public:
        HttpServletDispatch();
        void setDeafultServlet(MagicPtr<HttpServlet>& servlet);
        void addServlet(const std::string& name,MagicPtr<HttpServlet>& servlet);
        void handle(const std::shared_ptr<HttpSession>& session,MagicPtr<HttpRequest>& request,MagicPtr<HttpResponse>& response) override;
    private:
        MagicPtr<HttpServlet>& getMatchedServlet(const std::string& name);
    private:
        RWMutex m_Mutex{};
        MagicPtr<HttpServlet> m_DeafultServlet{};
        std::unordered_map<std::string,MagicPtr<HttpServlet>> m_Servlets{};
    };

}
}