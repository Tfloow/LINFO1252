#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>
#include <sem.h>


// 1.1 PHILOSOPHE

// Helper Function

int amount = 0;

int N;

void* philosophe ( void* arg ){
    int *id=(int *) arg;
    int left = *id;
    int right = (left + 1) % N; //PHILOSOPHES
    
    int i = 0;
    while(i<1000) {
        // philosophe pense
        if(left<right) {
            lock(&mut_arr[left]);
            lock(&mut_arr[right]);
        }else {
            lock(&mut_arr[right]);
            lock(&mut_arr[left]);
        }
        // Il mange
        amount++;
        unlock(&mut_arr[left]);
        unlock(&mut_arr[right]);
        i++;
    }
    return (NULL);
}

// ---MAIN FUNCTION----

void philosopher(int N){
    printf("=====Philosopher Problem (%d)=====\n", N);
    
    pthread_t* phil = (pthread_t*) malloc(sizeof(pthread_t)*N);
    mut_arr = my_mutex_init(N);

    if (mut_arr == NULL) {
        return;
    }
    int* ID;


    for(int i = 0; i < N; i++){

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


    free(phil);
    free(ID);

}

int main(int argc, char** argv){
    if(argc != 2){
        printf("Please provide the amount of philosopher you want\n");
        return EXIT_FAILURE;
    }

    N = (int)  atoi(argv[1]);
    philosopher((int)  atoi(argv[1]));
    printf("amount: %d\n", amount);
    return EXIT_SUCCESS;
}