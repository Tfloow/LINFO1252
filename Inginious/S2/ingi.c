#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>


void exchange_pointers(int** a, int** b){
   int* c = *a;

   *a = *b;
   *b = c;
}

void sort(void *base, size_t nel, size_t width, int (*compare)(const void *, const void *)){
    // Home made bubble sort
    char* test = (char*) malloc(width*nel);
    strcpy(test, (char*) base);
    // Check for error
    if(nel == 0 || width == 0 || base == NULL){
        return;
    } 

    // Init my variables and copy the array
    void* max; void* track;
    void* sorted = malloc(nel*width);
    void* tmpStorage = malloc(width);
    if(memcpy(sorted, base, nel) == NULL){
        return;
    }
    printf("%s\n", (char*) sorted);

    for(int i = 1; i < nel; i++){

        max = sorted;
        track = sorted+width;

        for(int j = 1; j < nel - 1; j++){
            printf("max: %c \t track: %c \t %s\n", *((char*) max), *((char*) track), (char*) sorted);
            // If our max is still the max compared to the next one
            if(compare(max, track) > 0){
                printf("%c is bigger than %c", *((char*) max), *((char*) track));
                memcpy(tmpStorage, max, width);
                memcpy(max, track, width);
                memcpy(track, tmpStorage, width);

            }
            max = track;
            track = max+width;
        }
    }

    printf("%s\n", (char*) sorted);

    memcpy(base, sorted, nel*width);
    printf("Copy\n");
}

int compare(const void* a, const void* b){
    if(a == NULL && b == NULL){
        return 0;
    }
    if(a == NULL){
        return -1;
    }
    if(b == NULL){
        return 1;
    }
    const char* stringa = a;
    const char* stringb = b;
    int val; int i = 1;

    char tracka = *stringa; char trackb = *stringb;

    while(tracka != '\0' || trackb != '\0'){

        if(tracka > 90){
            tracka-=32;
        }
        if(trackb > 90){
            trackb-=32;
        }

        val = tracka - trackb;

        if(val != 0){
            return val;
        }

        tracka = stringa[i]; trackb = stringb[i];
        i++;
    }

    if(tracka == '\0' && trackb == '\0'){
        return 0;
    }
    if(tracka == '\0'){
        return 1;
    }

    return -1;

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

    char* a = "Laura"; char* b = "laur";
    printf("%d\n", compare(a, b));

    a = "HellO"; b = "heLlo";
    printf("%d\n", compare(a, b));
}