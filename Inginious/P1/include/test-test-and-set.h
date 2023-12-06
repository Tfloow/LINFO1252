#ifndef _TEST_TEST_AND_SET_H_
#define _TEST_TEST_AND_SET_H_
#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>

int* mut_arr;

void lock(int* ver){
    //printf("mem: %p\n", ver);
    //printf("lock\n");
    //printf("%d\n", *ver);
    asm volatile(
            "movl $1, %%eax\n\t"
        "TEST_AND_SET:\n\t"
            "xchgl %%eax, %0\n\t"
            "testl %%eax, %%eax\n\t"
            "jnz VERROU\n\t" // if there is a fail with the set 
            "jmp EXIT\n\t" // if we successfully set the lock
        "VERROU:\n\t"
            "cmpl $0, %0\n\t" // if greater than 0 we are free
            "je TEST_AND_SET\n\t"
            "jmp VERROU\n\t"
        "EXIT:\n\t" // so we can safely exit the program
    :"+m"(*ver)
    :
    : "eax", "memory"
    );

//Inshasllah cette version est mieux:

///*
/*    while(verrou == 1){asm(
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
    



    ); //*/

}

//same as test-and-test algorithm:
void unlock(int* ver){
    //printf("unlock\n");
    //printf("%d\n", *ver);

    asm volatile(
        "movl $0, %%eax\n\t"
        "xchgl %%eax, %0\n\t"
        :"+m"(*ver)
        :
        : "eax", "memory"
    );
}

int* my_mutex_init(int amountLock) {
    int* ver = (int*) calloc(amountLock, sizeof(int));
    if (ver == NULL) {
        return NULL; 
    }
    return ver;
}


void test(){
    printf("ça marche \n");
}

#endif