#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

// Vous n'utiliserez pas le vrai heap mais un tableau accessible globalement déjà déclaré de la manière suivante:
uint8_t MY_HEAP[64000]; // max of 8 is 256 numbers

uint16_t cast(uint8_t a, uint8_t b){
    /*
    Cast 2 digits together to have a 16 bits number
    */
    uint16_t res = a << 8;
    res += b;
    return res;
}

void write16(uint8_t* arr, uint16_t index, uint16_t value){
    // Write in a 8 bits array 16 bits number. We write on 2 bits
    // Values is capped at 64 000 of size.
    if(index > 63998){
        printf(" [LOG]: Not enough space to write\n");
    }
    arr[index] = (value & 65280) >> 8;
    arr[index + 1] = (value & 255);
}

void dbgprint(uint8_t* arr, uint16_t amount){
    printf("[ ");
    for(int i = 0; i < amount; i++){
        printf("%d, ", arr[i]);
    }
    printf("]\n");
}

void init(){
    // Not compulsory just to prepare our fake heap
    static uint16_t first = 2; // the first value are used for the pointer
    write16(MY_HEAP, 0, 2);
    printf("%d\n", cast(MY_HEAP[0], MY_HEAP[1])); // to check we have correctly allocate the space, should say 2
    write16(MY_HEAP, 2, 63996); // Write the size left
    write16(MY_HEAP, 4, 0); // 0 address is for the variable first so it's our type of null

}

void my_free(void *pointer) {
    // Our free
}

void* my_malloc(size_t size){
    // Our malloc

    // Elementary check
    if(size % 2 != 0){
        printf(" [LOG]: we can only allocate multiples of 2\n");
    }

    uint16_t ptr = cast(MY_HEAP[0], MY_HEAP[1]); int32_t prev = -2; //only useful when flag == 1
    size_t found_size = cast(MY_HEAP[ptr], MY_HEAP[ptr+1]); // to get the first element's size
    uint8_t flag = 0; // flag to see if it's the first element we use 

    printf("%ld\n", found_size);
    while(found_size < size){
        //TODO
        printf("search");
        flag = 1;
        prev = ptr;
        ptr = cast(MY_HEAP[ptr+2], MY_HEAP[ptr+3]);

        // If no space found
        if(ptr < 2){
            printf(" [LOG]: no empty space found for this size of %ld\n", size);
            return NULL;
        }

        found_size = cast(MY_HEAP[ptr], MY_HEAP[ptr+1]);
    }

    write16(MY_HEAP, ptr, (uint16_t) size);
    write16(MY_HEAP, ptr + size + 2, (uint16_t) found_size - 2 - size); // Still need to check that my math are correct

    //TODO
    write16(MY_HEAP, prev + 2,  cast(MY_HEAP[prev+2], MY_HEAP[prev+3]) + size + 2);
    // We need to think about all possible case scenario and need to do extensive testing

    dbgprint(MY_HEAP, 20); // a print to debug the code

}

int main(int argc, char* argv[]){
    init();
    my_malloc(4);
    printf("%d\n", cast(249, 246)); // Seems okay
}