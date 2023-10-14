#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<math.h>
#include <stdint.h>



uint8_t MY_HEAP[64000]; // The heap where we allocate data

void write_wordtobytes(uint16_t nbr, uint8_t *a, uint8_t *b){
    /*
    Function to write 16 bits word on 2 bytes 

    nbr: the word we want to write
    a:   pointer to the higher weight bit
    b:   pointer to the lower weight bit
    */

    *a = nbr >> 8; //high order bits
    *b = nbr; //low order bits
}

uint16_t read_bytestoword(uint8_t a, uint8_t b){
    /*
    Function that reads 2 bits and return the result as a 16 bits

    a: value that represents the higher weight bit
    b: value that represents the lower order bit
    
    return: 16 bits data from a and b
    */
    
    uint16_t res = a << 8;
    res += b;
    return res;
}

void write_heap(uint8_t* arr, uint16_t index, uint16_t value){
    /*
    To write in a HEAP a VALUE at a specific INDEX

    arr:   an array that represents our heap
    index: where in the array to write the data
    value: what to write in the heap
    */

    uint8_t a; uint8_t b;
    write_wordtobytes(value, &a, &b);
    arr[index] = a;
    arr[index + 1] = b;
}

void init(){
    /**
    heap initialization: all bytes are free. 2 words of metadata on each side
    */

    write_heap(MY_HEAP, 0, 63996);
    write_heap(MY_HEAP, 63998, 63996);

}

void* my_malloc(size_t size){
    /*
    Function to allocate a specific size in the HEAP and gives back its index in the heap as a pointer

    size: size of the data we want to allocate

    return: a pointer to where our data was allocated
    */

    if(size>= 63996){
        printf("Requested size is too big");
        return NULL;
    }

    size_t searched_size = size;

    if(searched_size % 2 != 0){searched_size++;} //allocates multiples of 2 only
    searched_size = searched_size + 4; //adds metadatas size;

    int i = 0;

    uint16_t h = read_bytestoword(MY_HEAP[i], MY_HEAP[i+1]);

    /*
    Searches for big enough block. 3 cases to check
    -end of MY_HEAP reached
    -block big enough
    -block already allocated
    */

    while( i < 63998 && ( h < searched_size || ((h && 0x1) !=0) )){
        i = i + h + 4;
        h = read_bytestoword(MY_HEAP[i], MY_HEAP[i+1]);
    }

    //end of MY_HEAP without finding a block:
    if(i >= 63998){printf("End of MY_HEAP reached: couldn't allocate memory");}

    else{//stops at first big enough block, must check if block has to be divided then writes it as allocated (LOB = 1)

    if(h > searched_size + 4){ //+4 for extra metadatas from splitting

        uint16_t leftover = h - searched_size;
        //allocates:
        write_heap(MY_HEAP, i, searched_size + 1 - 4); //+1 : is allocated, -4 subtracts extra metadatas size
        
        write_heap(MY_HEAP, i + searched_size, leftover-4);

    }

    else{write_heap(MY_HEAP, i, searched_size + 1 - 4);} //block fits perfectly

    }
}


void my_free(void *pointer) {
    /*
    Free space that was previously allocated by my_malloc function

    pointer: a pointer to the area in the HEAP where we have allocated some data that we want now to free
    */
    // Our free
}



int main(int argc, char *argv[]){

    init();

    uint16_t size = read_bytestoword(MY_HEAP[63998], MY_HEAP[63999]);

    printf("%d\n", size);



    return 0;
}