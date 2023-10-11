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

void init(){
    // Not compulsory just to prepare our fake heap
    static first = 2; // the first value are used for the pointer
    write16(MY_HEAP, 0, 2);
    printf("%d\n", cast(MY_HEAP[0], MY_HEAP[1])); // to check we have correctly allocate the space
    write16(MY_HEAP, 2, 63996); // Write the size left
    write16(MY_HEAP, 4, 0); // 0 address is for the variable first so it's our type of null

}

void my_free(void *pointer) {
    // Our free
}

void* my_malloc(size_t size){
    // Our malloc
}
