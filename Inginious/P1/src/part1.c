#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdlib.h>


// 1.1 PHILOSOPHE

// Helper Function

void mange(int id) {
    // Eating (losing time)
    int a = 0;
    printf("Philosophe [%d] mange\n",id);
    for(int i=0;i< 100000; i++) {
        a = i;
    }
    a+=a; // to avoid unused-but-set-variable
}

void* philosophe ( void* arg, pthread_mutex_t* baguette){
    int *id=(int *) arg;
    int left = *id;
    int right = (left + 1);
    while(true) {
        printf("Philosophe [%d] pense\n",*id);
        pthread_mutex_lock(&baguette[left]);
        printf("Philosophe [%d] possède baguette gauche [%d]\n",*id,left);
        pthread_mutex_lock(&baguette[right]);
        printf("Philosophe [%d] possède baguette droite [%d]\n",*id,right);
        mange(*id);
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
    pthread_mutex_t* baguette = (pthread_mutex_t*) malloc(sizeof(pthread_mutex_t)*N);

    int arg = 0;
    philosophe(&arg, baguette);
    printf("%ln\n", phil);
}

int main(int argc, char** argv){
    philosopher(3);
}