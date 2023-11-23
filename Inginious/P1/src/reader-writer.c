#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdlib.h>
#include <string.h>


#define WRITE 640
#define READ  2560
#define THREAD_NUM 32

//2560 pour read et 640 pour write

pthread_mutex_t mutex_read_count;
pthread_mutex_t mutex_write_count;
pthread_mutex_t z;
sem_t wsem;
sem_t rsem;
int readcount = 0;
int writecount = 0;

/*
Problème actuel : nb_write et nb_read dépassent WRITE et READ et dépendent de THREAD_NUM (jsp si c grave faut demander)
*/
int nb_write = 0;
int nb_read = 0;






void *reader(void* rien){
    while(nb_read <READ){

        pthread_mutex_lock(&z);
        sem_wait(&rsem);

        pthread_mutex_lock(&mutex_read_count);
        readcount++;
        //printf("read %d\n", nb_read);
        //nb_read++;
        if(readcount == 1){sem_wait(&wsem);}

        pthread_mutex_unlock(&mutex_read_count);
        sem_post(&rsem);
        pthread_mutex_unlock(&z);

        //simulates read operation:
        for(int i = 0; i<10000; i++){}
        pthread_mutex_lock(&mutex_read_count);
        readcount --;
        //printf("read %d\n", nb_read);
        nb_read++;
        if(readcount == 0){sem_post(&wsem);}
        pthread_mutex_unlock(&mutex_read_count);

    }
    return NULL;
}




void* writer(void* rien){
    while(nb_write < WRITE){

        pthread_mutex_lock(&mutex_write_count);
        //Section critique
        writecount++;
        //printf("wrote %d\n", nb_write);
        //nb_write++;
        if(writecount ==1){sem_wait(&rsem);}
        pthread_mutex_unlock(&mutex_write_count);

        sem_wait(&wsem);
        //simulates: writing:
        for(int i = 0; i<10000; i++){}
        //printf("wrote %d\n", nb_write);
        //nb_write++;
        sem_post(&wsem);

        pthread_mutex_lock(&mutex_write_count);
        //Section critique
        writecount--;
        if(writecount == 0){sem_post(&rsem);}
        printf("wrote %d\n", nb_write);
        nb_write++;
        pthread_mutex_unlock(&mutex_write_count);
        }

    return NULL;
}




int main(int argc, char *argv[]){
    

//initialisation:
sem_init(&wsem, 0, 1);
sem_init(&rsem, 0, 1);
pthread_mutex_init(&mutex_read_count, NULL);
pthread_mutex_init(&mutex_write_count, NULL);
pthread_mutex_init(&z, NULL);

pthread_t thread[THREAD_NUM];

//creation:
for(int i = 0; i<THREAD_NUM; i++){
    if(i % 2 == 0){
        int err = pthread_create(&thread[i], NULL, &writer, NULL);
        if(err != 0){printf("Error creating writer\n");}
    }

    else{
        int err = pthread_create(&thread[i], NULL, &reader, NULL);
        if(err != 0){printf("Error creating reader\n");}
    }
}

//joins each thread:
for(int i = 0; i<THREAD_NUM; i++){
        int errjoin = pthread_join(thread[i], NULL);
        if(errjoin != 0){printf("error joining thread\n");}
    }




//destruction:
sem_destroy(&wsem);
sem_destroy(&rsem);
pthread_mutex_destroy(&mutex_read_count);
pthread_mutex_destroy(&mutex_write_count);
pthread_mutex_destroy(&z);

return 0;}