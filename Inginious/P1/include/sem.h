#ifndef _SEM_H_
#define _SEM_H_


//int verrou = 0;
int *sem_value;
int waiting_thr = 0;
int verrouSem = 0;
int verrou = 0;


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


void test(){

}

#endif