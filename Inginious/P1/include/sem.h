#ifndef _SEM_H_
#define _SEM_H_
#include <test-and-set.h>


//int verrou = 0;
int *sem_value;
int waiting_thr = 0;
int verrouSem = 0;
int verrou = 0;

void sem_lock(){
//Inshasllah cette version est mieux:

    while(verrou == 1){
        asm(
            "LOOP: \n\t"
            "movl $1, %%eax\n\t"
            :"+m"(verrou)
        );
    } 
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
void sem_unlock(){
    asm(
        "movl $0, %%eax\n\t"
        "xchgl %%eax, %0\n\t"
        :"+m"(verrou)
    );
}

// NEED TO REMOVE THE MY
int* my_my_sem_init(unsigned int value){
    sem_value = my_mutex_init(value);
    return sem_value;
}

int my_sem_wait(){
    if(*sem_value > 0){
        *sem_value = *sem_value -1;
    }
    else{//Thread must wait
        waiting_thr++;
        lock(&sem_value[0]);
    }
    return 0;
}

int my_sem_post(){
    if(waiting_thr >0){//one thread is freed
        unlock(&sem_value[0]);
        waiting_thr--;
    }else{
        *sem_value = *sem_value +1;
    }
    return 0;
}


void test(){

}

#endif