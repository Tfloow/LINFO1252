#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>


void exchange_pointers(int** a, int** b){
   int* c = *a;

   *a = *b;
   *b = c;
}


void printint(int* arr, int size){
    printf("[");
    for(int i = 0; i < size; i++){
        printf("%d, ", arr[i]);
    }
    printf("]\n");
}


void sort(void *base, size_t nel, size_t width, int (*compare)(const void *, const void *)){
    // Home made bubble sort
    // Check for error
    if(nel == 0 || width == 0 || base == NULL || compare == NULL){
        return;
    } 
    // Init my variables and copy the array
    void* max; void* track;
    void* sorted = malloc(nel*width);
    void* tmpStorage = malloc(width);
    if(sorted == NULL || tmpStorage == NULL){
        return;
    }
    if(memcpy(sorted, base, nel*width) == NULL){
        return;
    }
    printf("%s\n",(char*) sorted);
    for(int i = 0; i < nel; i++){

        max = sorted;
        track = sorted+width;

        for(int j = 1; j < nel - i; j++){
            // If our max is still the max compared to the next one
            if(compare(max, track) > 0){
                printf("%s\n",(char*) sorted);
                printf("Big\n");
                memcpy(tmpStorage, max, width);
                memcpy(max, track, width);
                memcpy(track, tmpStorage, width);
            }

            max = track;
            track = max+width;
        }
    }
    printf("Last copy\n");
    printf("%s\n", (char*) sorted);
    memcpy(base, sorted, nel*width);
}

int compare(const void* a, const void* b){
    const char* stringa = a;
    const char* stringb = b;
    int val; int vala; int valb;

    while ( *stringa != '\0' || *stringb != '\0')
    {
        vala = *stringa; valb = *stringb;
        if(*stringa > 96 && *stringa < 123){
            vala -= 32;
        }
        if(*stringb > 96 && *stringb < 123){
            valb -= 32;
        }
        val = vala - valb;

        if(val != 0){
            return val;
        }

        stringa++;
        stringb++;

    }
    
    if(*stringa == *stringb){
        return 0;
    }
    
    if(*stringa == '\0'){
        return -1;
    }

    if(*stringb == '\0'){
        return 1;
    }

    return 0; 
}

int intcmp(const void* a, const void* b){
    return *((int*) a) - *((int*)b);
}

int fold(int* tab, size_t nel, int start, int (*func)(int a, int b)){
    for(int i = 0; i < nel; i++){
        start = func(start, tab[i]);
    }

    return start;
}


void printTab(int* tab, size_t nel){
    printf("[");
    for(int i = 0; i < nel; i++){
        printf("%d, ", tab[i]);
    }
    printf("]\n");
}

int* map(int* tab, size_t nel, int (*func)(int a)){
    int* newArr = (int*) malloc(sizeof(int)*nel);

    for(int i = 0; i < nel; i++){
        newArr[i] = func(tab[i]);
    }

    return newArr;
}


int add(int a, int b){
    return a + b;
}

int doubling(int a){
    return 2*a;
}

int main(int argc, char* argv[]){

    //char* a = "Laura"; char* b = "laur";
    //printf("%d\n", compare(a, b));
    //
    //a = "HellO"; b = "heLlo";
    //printf("%d\n", compare(a, b));
    //
    //a = "a"; b = "A";
    //printf("%d\n", compare(a, b));
    //
    //a = "a"; b = "a";
    //printf("%d\n", compare(a, b));
    //
    char* test = " ZHFSaL^_}#!+-";
    sort(test, 14, sizeof(char), &compare);
    printf("%s\n", test);

}