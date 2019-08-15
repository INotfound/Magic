#pragma once
#include <cstdint>
#include <semaphore.h>
#include "../Util/Noncopyable.h"
namespace Magic {

class Semaphore : public Noncopyable{

public:
    Semaphore(uint32_t count =0);

    ~Semaphore();

    void wait();

    void notify();

private:
    sem_t m_Semaphore;
};

}

