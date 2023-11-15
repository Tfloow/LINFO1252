#include <pthread.h>
#include <semaphore.h>
#include <stdbool.h>
#include <stdio.h>

pthread_mutex_t mutex;
sem_t db;  // accès à la db
sem_t leave;
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
   pthread_mutex_lock(&mutex);
     // section critique
     readcount++;
     if (readcount==1)
     { // arrivée du premier reader
       sem_wait(&db);
     }
   pthread_mutex_unlock(&mutex);
   read_database();
   pthread_mutex_lock(&mutex);
     // section critique
     readcount--;
     if(readcount==0)
     { // départ du dernier reader
       sem_post(&db);
     }
   pthread_mutex_unlock(&mutex);
   process_data();

    if (want == 1){
        pthread_mutex_lock(&mutex);
        readcount--;
        pthread_mutex_unlock(&mutex);
        if(readcount == 0){
            sem_post(&db);
        }else{
            sem_wait(&leave);
        }
    }
 }
}


void writer(void)
{
 while(true)
 {
   prepare_data();

    want = 1;
    sem_wait(&leave);
   sem_wait(&db);
     // section critique, un seul writer à la fois
     write_database();
   sem_post(&db);

    int value;
    sem_getvalue(&leave, &value);
   for(int i = 0; i < -value; i++){
        sem_post(&leave);

   }
   }
 }

int main(int argc, char argv[]){
    printf("Hello world\n");
    int res = sem_init(&db, 0, 1);
    int newres = sem_init(&leave, 0, 1);
    int value;
    sem_getvalue(&db, &value);
    printf("%d\n", value);

}