#include "Magic/Magic.h"

void run(){

}

int main(void){
    Magic::Init();
    Magic::Thread thread("ok",run);
    getchar();
    return 0;
}