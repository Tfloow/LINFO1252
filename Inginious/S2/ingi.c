#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>


void exchange_pointers(int** a, int** b){
   int* c = *a;

   *a = *b;
   *b = c;
}

void sort(void *base, size_t nel, size_t width, int (*compar)(const void *, const void *))
{
    /*
    Need to be done
    */
}

int compar(const void* a, const void* b){
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


int main(int argc, char* argv[]){

    char* a = "Laura";
    char* b = "Laure";

    printf("%d\n", compar(a,b));

    return EXIT_SUCCESS;
}