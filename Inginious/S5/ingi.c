#include <sys/time.h>
#include <stdio.h>
#include <string.h>

/*
 * @pre: past_instant est un pointeur vers une structure timeval
 *       représentant un instant dans le passé.
 * @post: retourne le nombre de secondes écoulées depuis past_instant.
 */
time_t time_since(struct timeval *past_instant) {
    struct timeval tv;
    gettimeofday(&tv, NULL);


    return tv.tv_sec - past_instant->tv_sec;
}

int main(int argc, char* argv){
    struct timeval tv;
    gettimeofday(&tv, NULL);

    printf("%ld\n", time_since(&tv));
    return 0;
}