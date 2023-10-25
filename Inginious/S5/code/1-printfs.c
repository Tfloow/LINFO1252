#include <stdio.h>

int main() {
    int nombres[10] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    char* mots[10] = {"un", "deux", "trois", "quatre", "cinq", "six", "sept", "huit", "neuf", "dix"};
    for (int i = 0 ; i < 10 ; i++) {
	printf("%i s'ecrit %s, ", nombres[i], mots[i]);
    }
    for (int i = 0 ; i < 10 ; i++) {
	printf("%i s'ecrit %s\n", nombres[i], mots[i]);
    }
    return 0;
}

