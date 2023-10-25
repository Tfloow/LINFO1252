#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

int main() {
    if (mallopt(M_MMAP_THRESHOLD, 4*1024*1024*sizeof(long)) == 0) {
	printf("Vous avez une machine 32-bit ?!");
	exit(1);
    }
    printf("Debut des mallocs\n");
    char* tab[10000];
    int i;
    for (i = 0 ; i < 10000 ; i++) {
	tab[i] = malloc(1000);
	if (tab[i] == NULL) {
	    printf("Erreur d'allocation");
	    break;
	}
    }
    for (int j = 0 ; j < i ; j++) {
	free(tab[j]);
    }
    char* malloc_long[3];
    for (i = 0 ; i < 3 ; i++) {
	malloc_long[i] = malloc(100000);
	if (malloc_long[i] == NULL) {
	    printf("Erreur d'allocation");
	    break;
	}
    }
    for (int j = 0 ; j < i ; j++) {
	free(malloc_long[j]);
    }
    return 0;
}
