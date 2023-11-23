#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>

pthread_mutex_t* baguette;

// 1.1 PHILOSOPHE

// Helper Function



void* philosophe (void* args){
    int *id=(int *) args;
    int left = *id;
    int right = (left + 1);
    for(int i=0;i< 10000000; i++) {
        printf("Philosophe [%d] pense\n",*id);
        pthread_mutex_lock(&baguette[left]);
        printf("Philosophe [%d] possède baguette gauche [%d]\n",*id,left);
        pthread_mutex_lock(&baguette[right]);
        printf("Philosophe [%d] possède baguette droite [%d]\n",*id,right);
        pthread_mutex_unlock(&baguette[left]);
        printf("Philosophe [%d] a libéré baguette gauche [%d]\n",*id,left);
        pthread_mutex_unlock(&baguette[right]);
        printf("Philosophe [%d] a libéré baguette droite [%d]\n",*id,right);
    }
    return (NULL);
}

// ---MAIN FUNCTION----

void philosopher(int N){
    printf("=====Philosopher Problem (%d)=====\n", N);
    
    pthread_t* phil = (pthread_t*) malloc(sizeof(pthread_t)*N);
    baguette = (pthread_mutex_t*) malloc(sizeof(pthread_mutex_t)*N);

    for(int i = 0; i < N; i++){

        if (pthread_create(&(phil[i]),0, &philosophe, &i) != 0){
            printf("Issue with thread %d\n", i);
            return;
        }

    }
}

int main(int argc, char** argv){
    if(argc != 2){
        printf("Please provide the amount of philosopher you want\n");
        return EXIT_FAILURE;
    }

    philosopher((int)  atoi(argv[1]));
    return EXIT_SUCCESS;
}