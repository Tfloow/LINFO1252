#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "test-test-and-set.h"

//int verrou = 0;
int *sem_value;
int waiting_thr = 0;
int verrouSem = 0;

/*
void lock(){


//Inshasllah cette version est mieux:

    while(verrou == 1){asm(
        "LOOP: \n\t"
        "movl $1, %%eax\n\t"
        :"+m"(verrou)
            );} 
    //verrou == 0
    asm(
        "movl $1, %%eax\n\t"
        "xchgl %%eax, %0\n\t"
        "testl %%eax, %%eax\n\t"
        "jnz LOOP\n\t"
        :"+m"(verrou)
    



    ); //

}





//same as test-and-test algorithm:
void unlock(){
    asm(
        "movl $0, %%eax\n\t"
        "xchgl %%eax, %0\n\t"
        :"+m"(verrou)
    );
}
void init(int *sem_value, int value){
    *sem_value = value;
}

*/


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

return EXIT_SUCCESS;}