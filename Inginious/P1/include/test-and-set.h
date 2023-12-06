#ifndef _TEST_AND_SET_H_
#define _TEST_AND_SET_H_
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int* mut_arr;

void lock(int* ver) {
    //printf("mem: %p\n", ver);
    //printf("lock\n");


    asm volatile(
        "enter:\n\t"
        "movl $1, %%eax\n\t"
        "xchgl %%eax, %0\n\t"
        "testl %%eax, %%eax\n\t"
        "jnz enter\n\t"
        : "+m"(*ver)
        :
        : "eax", "memory"
    );
}

void unlock(int* ver) {
    //printf("unlock\n");

    asm volatile(
        "movl $0, %%eax\n\t"
        "xchgl %%eax, %0\n\t"
        : "+m"(*ver)
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

void my_mutex_destroy(){
    free(mut_arr);
}

#endif