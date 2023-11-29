#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>


int verrou = 0;
int NTHREADS;

void lock(){

    asm(
            "movl $1, %%eax\n\t"
    "ENTER:"
            "cmpl %%eax, %0\n\t"
            "je  ENTER\n\t" //keeps checking verrou's value until it is == 0 (!= 1)
            "jmp CHECK\n\t"


    "CHECK:"
            "xchgl %%eax, %0\n\t"
            "testl %%eax, %%eax\n\t"
            "jnz ENTER\n\t"
            //"ret \n\t"

    :"+m"(verrou)
    );

}





//same as test-and-test algorithm:
void unlock(){
    asm(
        "movl $0, %%eax\n\t"
        "xchgl %%eax, %0\n\t"
        :"+m"(verrou)
    );
}





void* myFunc(void* arg){

    for(int j = 0; j < 6400/NTHREADS; j++){
        lock();
        for(int a = 0; a <10000; a++){
            // DO
        }
        unlock();

    }


    return NULL;
}




void start(int NTHREADS){
    pthread_t* phil = (pthread_t*) malloc(sizeof(pthread_t)*NTHREADS);

    int* ID;
    
    for(int i = 0; i < NTHREADS; i++){
        ID = malloc(sizeof(int));
        *ID = i;

        if (pthread_create(&(phil[i]), NULL, myFunc, (void*) ID) != 0){
            printf("Issue with thread %d\n", i);
            return;
        }
    }

    for (int i = 0; i < NTHREADS; i++) {
        pthread_join(phil[i], NULL);
    }
}

int main(int argc, char** argv){
    if(argc != 2){
        printf("Please provide the amount of threads you want\n");
        return EXIT_FAILURE;
    }
    NTHREADS = (int)  atoi(argv[1]);
    start((int)  atoi(argv[1]));



    return EXIT_SUCCESS;
}