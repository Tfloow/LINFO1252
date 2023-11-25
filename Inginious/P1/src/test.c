#include <stdio.h>

int A = 0;

void* inc(){
    asm(
        "addl $1, %0\n\t"
        :"+m"(A)
    );
    printf("Valeur de A %d\n", A);
}


int main(int argc, char** argv){
    for(int i = 0; i < 10; i++){
        inc();
    }
}