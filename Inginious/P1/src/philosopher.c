#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>

pthread_mutex_t* baguette;

// 1.1 PHILOSOPHE

// Helper Function



void* philosophe ( void* arg ){
    printf("%d\n", *((int*) arg));
    int *id=(int *) arg;
    int left = *id;

    int right = 0;
    if(*id == 0){
        right = 1;
    }else{
        right = (left + 1) % *id; //PHILOSOPHES
    }
    int i = 0;
    while(i<10000000) {
        // philosophe pense
        if(left<right) {
            pthread_mutex_lock(&baguette[left]);
            pthread_mutex_lock(&baguette[right]);
        }else {
            pthread_mutex_lock(&baguette[right]);
            pthread_mutex_lock(&baguette[left]);
        }
        pthread_mutex_unlock(&baguette[left]);
        pthread_mutex_unlock(&baguette[right]);
        i++;
    }
    return (NULL);
}

// ---MAIN FUNCTION----

void philosopher(int N){
    printf("=====Philosopher Problem (%d)=====\n", N);
    
    pthread_t* phil = (pthread_t*) malloc(sizeof(pthread_t)*N);
    baguette = (pthread_mutex_t*) malloc(sizeof(pthread_mutex_t)*N);
    int* ID;


    for(int i = 0; i < N; i++){

        if (pthread_mutex_init(&baguette[i], NULL) != 0) { 
            printf("\n mutex init has failed\n"); 
            return; 
        } 

        ID = malloc(sizeof(int));
        *ID = i;


        if (pthread_create(&(phil[i]), NULL, philosophe, (void*) ID) != 0){
            printf("Issue with thread %d\n", i);
            return;
        }

    }

    // Wait for threads to finish
    for (int i = 0; i < N; i++) {
        pthread_join(phil[i], NULL);
    }

    for (int i = 0; i < N; i++){
        pthread_mutex_destroy(&(baguette[i]));
    }

    free(phil);
    free(baguette);
    free(ID);

}

int main(int argc, char** argv){
    if(argc != 2){
        printf("Please provide the amount of philosopher you want\n");
        return EXIT_FAILURE;
    }

    philosopher((int)  atoi(argv[1]));
    return EXIT_SUCCESS;
}