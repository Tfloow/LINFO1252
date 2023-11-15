#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>

pthread_mutex_t mutex;
sem_t wsem;  // accès à la db
sem_t rsem;
int readcount=0; // nombre de readers
int want = 0;


void read_database(){
    printf("Je lis\n");
}

void process_data(){
    printf("Miam les données\n");
}

void prepare_data(){
    printf("Préparation donnée\n");
}

void write_database(){
    printf("J'écris\n");
}

void reader(void)
{
 while(true)
 {

    sem_wait(&rsem);

   pthread_mutex_lock(&mutex);
     // section critique
     readcount++;
     if (readcount==1)
     { // arrivée du premier reader
       sem_wait(&wsem);
     }
   pthread_mutex_unlock(&mutex);

    sem_post(&rsem);

   read_database();
   pthread_mutex_lock(&mutex);
     // section critique
     readcount--;
     if(readcount==0)
     { // départ du dernier reader
       sem_post(&wsem);
     }
   pthread_mutex_unlock(&mutex);
   process_data();

 }
}


void writer(void)
{
 while(true)
 {
   prepare_data();

    sem_wait(&rsem);
   sem_wait(&wsem);
     // section critique, un seul writer à la fois
     write_database();
   sem_post(&wsem);
   sem_post(&rsem);

 }
}

int main(int argc, char argv[]){
    printf("Hello world\n");
    int res = sem_init(&wsem, 0, 1);
    sem_init(&rsem, 0, 1);

}