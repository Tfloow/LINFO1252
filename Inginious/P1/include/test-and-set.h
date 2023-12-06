#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#ifndef _TEST_AND_SET_H_
#define _TEST_AND_SET_H_

int L = 0;
int NTHREADS;

void* lock(){
    asm(
        "enter:\n\t"
        "movl $1, %%eax\n\t"
        "xchgl %%eax, %0\n\t"
        "testl %%eax, %%eax\n\t"
        "jnz enter\n\t"
        :"+m"(L)
    );

    return NULL;
}

void* unlock(){
    asm(
        "movl $0, %%eax\n\t"
        "xchgl %%eax, %0\n\t"
        :"+m"(L)
    );
    return NULL;
}

#endif