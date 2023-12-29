

# Cours 7

## Sémaphores et Compléments sur les Threads

### Sémaphore

Une sémaphore est donc une structure qui contient:
- Entier non-signé: positif ou nul
- File vers les pointeurs de threads en attente (*Blocked*)

```c
#include <semaphore.h> 
int sem_init(sem_t *sem, int pshared, unsigned int value); // initialise avec value le nombre de place
//pshared = 0
int sem_destroy(sem_t *sem); // destruction du semaphore 
int sem_wait(sem_t *sem); // Attente place
int sem_post(sem_t *sem); // Liberation place
```

Utile pour les problèmes de rendez-vous, producteurs-consommateurs, lecteurs-écrivains, ...

Donc une sémaphore avec `value = 1` est une sorte de mutex. Attention, un autre thread peut libérer notre sémaphore ce qui est différent d'un mutex où c'est généralement le même thread appelle `lock()` et `unlock()`.

#### Problème du Rendez-Vous

2 phases:
1. N threads s'exécutent indépendamment
2. Commence quand tous les threads ont finit la phase 1. (on peut utiliser la structure `pthread_barrier_init`)

```c
sem_t rendezvous; // Bloque les threads en attente de RDV 
pthread_mutex_t mutex; // Protège accès à count
int count=0; // Nombre de threads ayant atteint le point de RDV

sem_init(&rendezvous,0,0)

premiere_phase(); 

// section critique 
pthread_mutex_lock(&mutex); count++; 
if(count==N) {
    // tous les threads sont arrivés 
    sem_post(&rendezvous);
}
pthread_mutex_unlock(&mutex); 
// attente à la barrière 
sem_wait(&rendezvous);
// libération d'un autre thread en attente 
sem_post(&rendezvous);

seconde_phase();
```

#### Producteurs-Consommateurs

On a un ensemble de threads qui produisent quelque chosent et d'autres qui vont le consommer. Les producteurs vont donc placer leurs données dans une zone partagée et les consommateurs vont pouvoir se servir.

```c
// Initialisation 
#define N 10 
// places dans le buffer 
pthread_mutex_t mutex; 
sem_t empty; 
sem_t full;

pthread_mutex_init(&mutex, NULL); 
sem_init(&empty, 0 , N); // buffer vide
sem_init(&full, 0 , 0); // buffer vide

// Producteur
void producer(void) {
    int item; 
    while(true) {
        item=produce(item); 
        sem_wait(&empty); // attente d'une place libre 
        pthread_mutex_lock(&mutex); 
        // section critique 
        insert_item();
        pthread_mutex_unlock(&mutex);
        sem_post(&full); // une place remplie
    }
}

// Consommateur
void consumer(void) {
    int item; 
    while(true) {
        sem_wait(&full); // attente d'une place remplie 
        pthread_mutex_lock(&mutex); 
        // section critique 
        item = remove(item);
        pthread_mutex_unlock(&mutex);
        sem_post(&empty); // une place libre
    }
}
```

#### Reader-writers

```c
pthread_mutex_t mutex; 
sem_t db; // accès à la db 
int readcount=0; // nombre de readers

sem_init(&db, NULL, 1)

void writer(void) {
    while(true) {
        prepare_data();
        sem_wait(&db);
        // section critique, un seul writer à la fois 
        write_database(); 
        sem_post(&db); 
    }
}

void reader(void) {
    while(true) {
        pthread_mutex_lock(&mutex); 
        // section critique 
        readcount++; 
        if (readcount==1) { 
            // arrivée du premier reader 
            sem_wait(&db);
        }
        pthread_mutex_unlock(&mutex); 
        read_database();
        pthread_mutex_lock(&mutex); 
        // section critique 
        readcount--; 
        if(readcount==0) { 
            // départ du dernier reader 
            sem_post(&db);
        }
        pthread_mutex_unlock(&mutex); 
        process_data();
    }
}
```

On assure bien que l'écrivain ne peut être en section critique en même temps qu'un lecteur. On a un accès parallèle pour les écrivains. On risque juste d'avoir un writer qui attend indéfiniment que tous les écrivains se libèrent, problème de **famine**.

### Variable `volatile`

Quand on accède à une variable en C, C va mémoriser la valeur. On peut s'assurer qu'il relise la valeur à chaque fois en ajoutant le mot-clé `volatile`. **Ce n'est pas une solution pour se protéger des écritures et accès concurrent !**

### Threads-safe

Une fonction est dite thread-safe si des threads en concurrences peuvent utiliser correctement une même fonction. Si elle utilise des variables globales ou statiques ❌.

## Les processus

*Définition*: Instructions à exécuter par les processeurs. Cela peut avoir plusieurs contextes d'exécutions. Chaque processus à son **espace mémoire spécifique**. Un segment *stack* par contexte d'exécution. Il a également des méta-données permettant au SE de le contrôler.

### Librairies

On utilise souvent des librairies en C sous la forme de header `math.h`. Un processus ne contient généralement pas le code des librairies. On utilise donc les librairies partagées du SE (un peu comme les fichiers midi).

### Création d'un processus

Les étapes:
1. Le shell doit localiser le fichier exécutable dans le `PATH`
2. Demande au kernel de créer un nouveau processus
3. Le processus demande au kernel le chargement et l'exécution du code du fichier programme
4. On peut bloqué le shell et attendre de récupérer la valeur de retour

On peut créer de nouveau processus via un `fork`. Cela fait des **copies** distinctes qui ne partagent rien entre elles. L'identifiant du processus va être différent. Le retour du père est 0 et des fils > 0.

```c
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h>

int g=0; // segment données

int main (int argc, char *argv[]) { 
    int l=1252; // sur la pile 
    int *m;     // sur le heap
    m=(int *) malloc(sizeof(int)); 
    *m=-1;

    pid_t pid; 
    pid=fork();
    
    if (pid==-1) {
    // erreur à l'exécution de fork 
    perror("fork"); 
    exit(EXIT_FAILURE);
    }
    // pas d'erreur 
    if (pid==0) {
        // processus fils 
        l++; g++; *m=17; 
        printf("Dans le processus fils g=%d, l=%d et *m=%d\n", g,l,*m); 
        free(m); 
        return(EXIT_SUCCESS); 
    } else {
        // processus père 
        sleep(2); 
        printf("Dans le processus père g=%d, l=%d et *m=%d\n",g,l,*m);
        free(m); 
        // ... 
        return(EXIT_SUCCESS);
    }
}
```

Output:

```
Dans le processus fils g=1, l=1253 et *m=17 
Dans le processus père g=0, l=1252 et *m=-1
```

On a aussi les *PID* qui sont les identificateurs des processus. Le PID 1 est le processus *init* donc au démarrage du système.

### Flux Standards et de Partage

On a un partage entre processus père et fils de *STDOUT* et *STDERR*. *STDIN* est soit conservé ou délégué par le processus père. Ils écrivent donc dans la même sortie.

`printf` ne fait pas d'appel bloquant mais il fait du **buffering** donc n'imprime pas tout de suite. Ainsi il ne fait qu'un seul appel à `write` par ligne !

### Fin d'un Processus

Pour mettre fin à un processus, on utilise `exit()` ou un `retur(0)` (implicite par le compilateur). On a toujours la valeur du processus père et on peut spécifier des actions à faire à la terminaison `atexit()` (on spécifie une fonction qu'on veut réaliser). `exit()` fait un appel système `exit`.

Pour récupérer une valeur de retour, on utilise:

```c
#include <sys/types.h> 
#include <sys/wait.h>

pid_t waitpid(pid_t pid, int *status, int options);
// pid:     identifiant fils
// status:  entier qui contiendra la valeur
// options: des options
```

### Processus Orphelin et Zombie

- *Orphelin*: Si un processus père n'attend pas un processus fils, il sera rattaché au PID 1. 
- *Zombie* : Si un processus à terminer mais que sa valeur de retour n'a jamais été récupérée, le kernel doit la stocker.

### Exécution d'un programme

Pour éviter d'être limité à des exécutions Père-Fils où le fils à le même code que celui du père et une copie de ses données, on peut remplacer la mémoire de ce processus via celle dans un fichier exécutable comme ceci:

```c
#include <unistd.h> 

int execve(const char *path, char *const argv[], char *const envp[]);
// path: l'espace mémoire à utiliser
// argv: les arguments à passer
// envp: les informations vis-à-vis de l'environnement
```

Pour consulter tous les processus en cours, on utilise `ps`, `top` (gestionnaire de tâche), `pstree` (voir les relations père-fils). Tout cela va provenir de `/proc` (pseudo système de fichier).


