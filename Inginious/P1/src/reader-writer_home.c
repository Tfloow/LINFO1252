#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sem.h>

#define WRITE 640
#define READ  2560
//#define THREAD_NUM 32

//2560 pour read et 640 pour write

pthread_mutex_t mutex_read_count;
pthread_mutex_t mutex_write_count;
pthread_mutex_t z;

int* mutex; // 0 is read, 1 is write and 2 is z

my_sem_t wsem;
my_sem_t rsem;
int readcount = 0;
int writecount = 0;

/*
Problème actuel : nb_write et nb_read dépassent WRITE et READ et dépendent de THREAD_NUM
*/
int nb_write = 0;
int nb_read = 0;


void *reader(void* rien){
    while(true){


        lock(&mutex[2]);
        my_sem_wait(&rsem);

        lock(&mutex[0]);
        readcount++;


        if(readcount == 1){my_sem_wait(&wsem);}

        unlock(&mutex[0]);
        my_sem_post(&rsem);
        unlock(&mutex[2]);

        //simulates read operation:
        for(int i = 0; i<10000; i++){}


        lock(&mutex[0]);
        //section critique
        readcount--;
        //indique qu'une lecture en plus a été faite:
        if(nb_read == READ){
        if(readcount == 0){my_sem_post(&wsem);}
        unlock(&mutex[0]);
        return EXIT_SUCCESS;
    }


        nb_read++;
        //printf("read number %d\n", nb_read);

        if(readcount == 0){my_sem_post(&wsem);}
        unlock(&mutex[0]);
    }
    return NULL;
}




void* writer(void* rien){
    while(true){



        lock(&mutex[1]);
        //Section critique
        writecount++;
    

        if(writecount ==1){my_sem_wait(&rsem);}
        unlock(&mutex[1]);

        my_sem_wait(&wsem);
        //simulates: writing:
        for(int i = 0; i<10000; i++){}

        if(nb_write == WRITE){
            my_sem_post(&wsem);

        lock(&mutex[1]);
        //Section critique
        writecount--;


        

        if(writecount == 0){my_sem_post(&rsem);}

        unlock(&mutex[1]);
        return EXIT_SUCCESS;}


        nb_write++;
        //printf("write number %d\n", nb_write);


        my_sem_post(&wsem);

        lock(&mutex[1]);
        //Section critique
        writecount--;


        

        if(writecount == 0){my_sem_post(&rsem);}

        unlock(&mutex[1]);
        }

    return NULL;
}




int main(int argc, char *argv[]){
        

    //initialisation:
    mutex = my_mutex_init(3);
    wsem = *((my_sem_t*) malloc(sizeof(my_sem_t)));
    rsem = *((my_sem_t*) malloc(sizeof(my_sem_t)));
    my_sem_init(&wsem, 0, 1);
    my_sem_init(&rsem, 0, 1);


    int THREAD_NUM = (int)  atoi(argv[1]);

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
        if(errjoin != 0){
            printf("error joining thread\n");
        }
    }

    printf("wrote: %d, read: %d\n", nb_write, nb_read);
    printf("space in write: %d \t space in read: %d\n", wsem.amount, rsem.amount);

    //destruction:
    my_mutex_destroy();

    return 0;
}