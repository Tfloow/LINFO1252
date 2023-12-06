#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "sem.h"


void init(int *sem_value, int value){
    *sem_value = value;
}



void wait(){

    if(*sem_value > 0){*sem_value = *sem_value -1;}

    else{//Thread must wait
    waiting_thr++;
    lock();
    }

}



void post(int *sem_value){

    if(waiting_thr >0){//one thread is freed
    unlock();
    waiting_thr--;}

    else{*sem_value = *sem_value +1;}

}


int main(int argc, char** argv){

    test();

    return EXIT_SUCCESS;
}
