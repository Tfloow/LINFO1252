#include "test-and-set.h"

int NTHREADS;
//int testing = 0;

void* myFunc(void* arg) {
    int* mutex = (int*)arg;

    for (int j = 0; j < 6400 / NTHREADS; j++) {
        lock(mutex);
        for (int a = 0; a < 10000; a++) {
            // DO
        }
        //testing++;
        unlock(mutex);
    }

    return NULL;
}

void start(int NTHREADS) {
    pthread_t* threads = (pthread_t*)malloc(sizeof(pthread_t) * NTHREADS);
    mut_arr = my_mutex_init(1);

    if (mut_arr == NULL) {
        return;
    }

    for (int i = 0; i < NTHREADS; i++) {
        if (pthread_create(&threads[i], NULL, myFunc, (void*) &mut_arr[0]) != 0) {
            printf("Issue with thread %d\n", i);
            return;
        }
    }

    for (int i = 0; i < NTHREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    free(mut_arr);
    free(threads);
}

int main(int argc, char** argv) {
    if (argc != 2) {
        printf("Please provide the amount of threads you want\n");
        return EXIT_FAILURE;
    }

    NTHREADS = atoi(argv[1]);
    start(NTHREADS);

    //printf("testing: %d\n", testing);

    return EXIT_SUCCESS;
}

