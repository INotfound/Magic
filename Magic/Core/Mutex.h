#pragma once
#include "Core.h"
#include "Adapter/Adapter.h"

namespace Magic{

class Semaphore : public Noncopyable{
public:
    Semaphore(uint32_t count =0);

    ~Semaphore();

    void wait();

    void notify();
private:
    SemType m_Semaphore;
};

}