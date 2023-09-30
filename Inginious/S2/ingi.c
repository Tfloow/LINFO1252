#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>


void exchange_pointers(int** a, int** b){
   int* c = *a;

   *a = *b;
   *b = c;
}

void sort(void *base, size_t nel, size_t width, int (*compare)(const void *, const void *))
{
    // Going to implement the worst sorting algorithm ever
    void* cur = base; void* track = base; char* new  = malloc(width*nel);
    memcpy(new, (char*) base, nel); // copy the old in the new
    void* min; int place; char* tmp = (char*) malloc(width);

    for(int i = 0; i < nel; i++){
        // so we start with the first element of the non sorted subarray
        min = track; // the minimum at the start is the first element
        printf("%s \t %s\n", (char*)min, (char*) cur);
        for(int j = i; j < nel; j++){
            if(compare(min, cur) > 0){
                min = cur;
                place = j;
            }
            cur = cur + width;
        }
        memcpy(tmp, min, width);
        new[i] = *tmp;

        printf("%c \t %c \t %s\n", *tmp,*((char*) min), new);
        track = (void*)track + width;
        cur = track;
    }
    memcpy(base, (void*) new, nel);
}

int compare(const void* a, const void* b){
    const char* stringa = a;
    const char* stringb = b;

    while ( *stringa != '\0' && *stringb != '\0')
    {
        if(*stringa < *stringb){
            return -1;
        }

        if(*stringb < *stringa){
            return 1;
        }

        stringa++;
        stringb++;

    }

    if(*stringa == '\0'){
        return -1;
    }

    if(*stringb == '\0'){
        return 1;
    }

    return 0; 
}

void* fold(void* tab, size_t nel, void* res, void* (*func)(void* a, void* b)){
    int* tabw = (int*) tab;

    if(nel <= 1){
        return tab;
    }

    return NULL;
}

void* map(void* tab, size_t nel, int (*func)(void* a)){
    return NULL;
}


int main(int argc, char* argv[]){



    return EXIT_SUCCESS;
}