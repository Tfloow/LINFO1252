#include <stdio.h>

int L = 0;

void lock(){
    asm(
        "enter:\n\t"
        "movl $1, %%eax\n\t"
        "xchgl %%eax, %0\n\t"
        "testl %%eax, %%eax\n\t"
        "jnz enter\n\t"
        :"+m"(L)
    );
}



int main(int arc, char** argv){
    printf("Hello\n");
    lock();
}