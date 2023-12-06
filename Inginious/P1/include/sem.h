#ifndef _SEM_H_
#define _SEM_H_
#include <test-and-set.h>

typedef struct
{
    int pshared;
    int amount;
    int* locked;
} my_sem_t;

//int verrou = 0;
int* sem_value;
/*
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
}*/

// NEED TO REMOVE THE MY
int* my_sem_init(my_sem_t* sem, int pshared, unsigned int value){
    //sem = (my_sem_t*) malloc(sizeof(my_sem_t));
    if(sem == NULL){
        return NULL;
    }
    // pshared is just a dummy variable
    sem->amount = value;
    sem->pshared = 0;
    sem->locked = my_mutex_init(1);
    //printf("%p\n", sem->locked);
    //printf("%d\n", *(sem->locked));

    sem_value = my_mutex_init(value); // amount of semaphore we want
    return sem_value;
}

int my_sem_wait(my_sem_t* sem){
    //printf("%p\n", sem->locked);
    //printf("%d\n", *(sem->locked));
    //printf("wait\tstart\n");

    while(sem->amount <= 0){

    }
    lock(sem->locked);

    sem->amount--;

    unlock(sem->locked);
    //printf("w   \tfinish\n");
    return 0;
}

int my_sem_post(my_sem_t* sem){
    //printf("post\tstart\n");
    lock(sem->locked);

    sem->amount++;

    unlock(sem->locked);

    //printf("p   \tfinish\n");
    return 0;
}



void test(){

}

#endif