#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sem.h>


// Initialisation
#define N 8 // places dans le buffer
#define NB_PRODUCT 8192


//protected variables:
int buffer[8];
int count = 0;
int item_produced = 0;
int item_consumed = 0;

int* mutex;
my_sem_t empty;
my_sem_t full;




// Producteur
void* producer(void* rien){
    while(true){

        //simulates production:
        for(int i = 0; i<10000; i++){
        }


        my_sem_wait(&empty); // attente d’une place libre

        lock(mutex);
        // section critique


        if(item_produced == NB_PRODUCT){
            unlock(mutex);
            my_sem_post(&full);
            return EXIT_SUCCESS;
        }


        buffer[count] = item_produced;
        count++;
        item_produced++;
        unlock(mutex);


        my_sem_post(&full); // il y a une place remplie en plus
    }

    return NULL;
}



// Consommateur
void* consumer(void* rien){
    int product;
    while(true){


    my_sem_wait(&full); // attente d’une place remplie

    lock(mutex);
    // section critique

        if(item_consumed == NB_PRODUCT){
            unlock(mutex);
            my_sem_post(&empty);
            return EXIT_SUCCESS;
    }

    product = buffer[count - 1];
    count--;
    item_consumed++;



    unlock(mutex);

    my_sem_post(&empty); // il y a une place libre en plus

    //printf("consumed %d\n", product);

    //simulates consumption:
    for(int i = 0; i<10000; i++){}

    }
    product -= product; // DUMMY TO BE DELTED
    return NULL;
}



int main(int argc, char *argv[]){


    mutex = my_mutex_init(1);
    empty = *((my_sem_t*) malloc(sizeof(my_sem_t)));
    full = *((my_sem_t*) malloc(sizeof(my_sem_t)));

    my_sem_init(&empty, 0 , N); // buffer vide
    my_sem_init(&full, 0 , 0); // buffer vide

    printf("%p\n", empty.locked);
    printf("%d\n", *(empty.locked));


    int THREAD_NUM = (int)  atoi(argv[1]);

    pthread_t thread[THREAD_NUM];




        //initialisation: half producer, half consumer
        for(int i = 0; i<THREAD_NUM; i++){

            if(i % 2 == 0){
                int err = pthread_create(&thread[i], NULL, &producer, NULL);
                if(err != 0){printf("error creating producer\n");}
            }

            else{
                int err = pthread_create(&thread[i], NULL, &consumer, NULL);
                if(err != 0){printf("error creating consumer\n");}
            }

        }


        //joins each thread
        for(int i = 0; i<THREAD_NUM; i++){
            int errjoin = pthread_join(thread[i], NULL);
            if(errjoin != 0){printf("error joining thread\n");}
        }


        //printf("Produced: %d, Consumed: %d\n", item_produced, item_consumed);
        

    //destroys semaphore and mutex at the end:
    my_mutex_destroy();

    printf("space in empty: %d \t space in full: %d\n", empty.amount, full.amount);

    return 0;
}