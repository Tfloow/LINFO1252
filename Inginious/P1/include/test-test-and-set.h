#ifndef _TEST_TEST_AND_SET_H_
#define _TEST_TEST_AND_SET_H_

int verrou = 0;
int NTHREADS;

void lock(){

    asm(
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

    :"+m"(verrou)
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
void unlock(){
    asm(
        "movl $0, %%eax\n\t"
        "xchgl %%eax, %0\n\t"
        :"+m"(verrou)
    );
}


void test(){
    printf("ça marche \n");
}

#endif