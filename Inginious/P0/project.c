#include <stdint.h>
#include <stddef.h>

// Vous n'utiliserez pas le vrai heap mais un tableau accessible globalement déjà déclaré de la manière suivante:
uint8_t MY_HEAP[64000];

void init(){
    // Not compulsory just to prepare our fake heap
}

void my_free(void *pointer) {
    // Our free
}

void* my_malloc(size_t size){
    // Our malloc
}
