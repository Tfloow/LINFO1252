# Réponses

- [Réponses](#réponses)
  - [Problèmes des threads](#problèmes-des-threads)
  - [Améliorer le tout](#améliorer-le-tout)
    - [Solution proposée](#solution-proposée)


## Problèmes des threads

```c
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
 }
}
```

```c
void writer(void)
{
 while(true)
 {
   prepare_data();
   sem_wait(&db);
     // section critique, un seul writer à la fois
     write_database();
   sem_post(&db);
   }
 }
```

![Alt text](image.png)

## Améliorer le tout

On doit utiliser des sémaphores, des mutex etc.
Faut voir [ici](better.c)

### Solution proposée