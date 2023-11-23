#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdlib.h>
#include <string.h>


// Initialisation
#define N 8 // places dans le buffer
#define THREAD_NUM 16
#define NB_PRODUCT 8192


//protected variables:
int buffer[8];
int count = 0;
int item_produced = 0;
int item_consumed = 0;

pthread_mutex_t mutex;
sem_t empty;
sem_t full;




// Producteur
void* producer(void* rien){
    while(item_produced < NB_PRODUCT){

        //simulates production:
        for(int i = 0; i<10000; i++){
        }


        sem_wait(&empty); // attente d’une place libre

        pthread_mutex_lock(&mutex);
        // section critique
        buffer[count] = item_produced;
        count++;
        item_produced++;
        pthread_mutex_unlock(&mutex);


        sem_post(&full); // il y a une place remplie en plus
    }

    return NULL;
}



// Consommateur
void* consumer(void* rien){
    int product;
    while(item_consumed  < NB_PRODUCT){


    sem_wait(&full); // attente d’une place remplie

    pthread_mutex_lock(&mutex);
    // section critique
    product = buffer[count - 1];
    count--;
    item_consumed++;
    pthread_mutex_unlock(&mutex);

    sem_post(&empty); // il y a une place libre en plus

    printf("consumed %d\n", product);

    //simulates consumption:
    for(int i = 0; i<10000; i++){}

    }
    return NULL;
}








int main(int argc, char *argv[]){
pthread_mutex_init(&mutex, NULL);
sem_init(&empty, 0 , N); // buffer vide
sem_init(&full, 0 , 0); // buffer vide
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

//destroys semaphore and mutex at the end:
sem_destroy(&empty);
sem_destroy(&full);
pthread_mutex_destroy(&mutex);
return 0;}