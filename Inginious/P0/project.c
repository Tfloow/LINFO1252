#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdint.h>

uint8_t MY_HEAP[64000]; // The heap where we allocate data

// Write Function
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

uint16_t isFree(uint8_t* arr, uint16_t index){
    /*
    Check if a spot is free in the heap

    arr:   the HEAP
    index: the index with the meta data

    return: 0 if NOT free
    */
    uint16_t meta = read_bytestoword(arr[index], arr[index+1]);
    return !(meta % 2);
}

// Debug function
void dbgprint(uint8_t* arr, int amount){
    /*
    Print the first AMOUNT elements of the ARR

    arr:    the HEAP
    amount: the amount we want to see
    */
    printf("[ "); int flag = 0;

    int offset = 0;
    if(amount < 0){
        offset = 64000+amount;
        amount = -amount;
        flag = 1;
        printf(" ..., ");
    }


    for(int i = 0; i < amount; i++){
        printf("%d, ", arr[offset + i]);
    }

    if(!flag){
        printf("... ");
    }

    printf("]\n");
}

void init(){
    /**
    heap initialization: all bytes are free. 2 words of metadata on each side
    */

    // We use ODD NUMBERS because it shows if the upcoming bits are free or unoccupied
    write_heap(MY_HEAP, 0, 63996);
    write_heap(MY_HEAP, 63998, 63996);

}

void* my_malloc(size_t size){
    /*
    Function to allocate a specific size in the HEAP and gives back its index in the heap as a pointer

    size: size of the data we want to allocate

    return: a pointer to where our data was allocated
    */

    if(size> 63996){
        printf("Requested size is too big\n");
        return NULL;
    }

    size_t searched_size = size;

    if(searched_size % 2 != 0){searched_size++;} //allocates multiples of 2 only
    searched_size = searched_size + 4; //adds metadatas size;

    int i = 0;

    uint16_t h;

    /*
    Searches for big enough block. 3 cases to check
    -end of MY_HEAP reached
    -block big enough
    -block already allocated
    */

    // Need to recheck if the loop is correct or not
    while( i < 63996 && ( h < searched_size || ((h && 0x1) !=0) )){ 

        h = read_bytestoword(MY_HEAP[i], MY_HEAP[i+1]);
        //if(h %2 != 0){i--;}

        if(isFree(MY_HEAP, i) && ((h >= searched_size + 4) || (h == searched_size))){
            printf("Found space\n");
            if(h %2 != 0){i--;}
            break;
        }
        i = i + h + 4;
        if(h %2 != 0){i--;}
    }


    //end of MY_HEAP without finding a block:
    if(i >= 63998){
        printf("End of MY_HEAP reached: couldn't allocate memory\n");
        return NULL;
    }

    //stops at first big enough block, must check if block has to be divided then writes it as allocated (LOB = 1)

    if(h >= searched_size + 4){ //+4 for extra metadatas from splitting

        uint16_t leftover = h - searched_size;
        //allocates and writes metadatas:
        write_heap(MY_HEAP, i, searched_size + 1 - 4); //+1 : is allocated, -4 subtracts extra metadatas size
        write_heap(MY_HEAP, i + searched_size - 2, searched_size + 1 - 4);

        //adjusts rest of the block's values:
        write_heap(MY_HEAP, i + searched_size, leftover);
        write_heap(MY_HEAP, i +searched_size + leftover + 2 , leftover);

        //printf("Beginnning index:%d\n",i +searched_size);
        //printf("End index: %d\n",i +searched_size + leftover);

    }else{
        write_heap(MY_HEAP, i, searched_size + 1 - 4); //block fits perfectly
        write_heap(MY_HEAP, i + searched_size - 2, searched_size + 1 - 4);
    }
    
    return ((void*) &(MY_HEAP[i+2]));
}


void my_free(void *ptr) {
    /*
    Free space that was previously allocated by my_malloc function

    ptr: a pointer to the area in the HEAP where we have allocated some data that we want now to free
    */
    // Our free

    /*
     4 cases:
    - prev and next are allocated: free ptr only
    -prev is free, next is allocated: merge ptr and free
    -prev is allocated, next is free: merge ptr and next
    -prev and next are free: merge prev ptr and free
    */


    //gets ptr size:
    uint8_t* ptr_cast = (uint8_t *) ptr;
    uint16_t size = read_bytestoword(*(ptr_cast -2),*(ptr_cast - 1) ) - 1;

    //gets next block size
    uint16_t next = read_bytestoword(*(ptr_cast + size +2), *(ptr_cast + size + 3));
    uint16_t prev = 1; //read_bytestoword(*(ptr_cast - 4), *(ptr_cast - 3));

    write_wordtobytes(63000, (ptr_cast -2),(ptr_cast - 1));

    if(next % 2 == 0 && prev % 2 != 0){



    /*
    +4: we get back the bits used at the end of ptr and at the beginning of next 
    */

    write_wordtobytes(size + next + 4, ptr_cast -2,ptr_cast - 1);
    /*
    + 3: skips 2nd bit of ptr size and next's metadata
    */
    write_wordtobytes(size + next + 4, ptr_cast + size + 3 + next + 1,  ptr_cast + 3 + size + next + 2);
        
    }


    else if(next % 2 == 0 && prev % 2 == 0){}

    else if(next % 2 != 0 && prev % 2 == 0){}


    //free ptr only: just subtract 1
    else{

        write_wordtobytes(size - 1, ptr_cast - 2, ptr_cast - 1);
        write_wordtobytes(size - 1, ptr_cast + size , ptr_cast + size + 1);
    }










}



int main(int argc, char *argv[]){

    init();


    uint8_t* test = (uint8_t*) my_malloc(4); // Need to cast it in 8 bit watch out not int !
    uint8_t* test2 = (uint8_t*) my_malloc(4);
    uint8_t* test3 = (uint8_t*) my_malloc(4);


    //printf("%d\n", read_bytestoword(MY_HEAP[24], MY_HEAP[25]));
    //printf("%d\n", read_bytestoword(MY_HEAP[63998], MY_HEAP[63999]));


    //my_free(test);

    for(int i = 0; i< 30; i++){
       printf("%d\n", MY_HEAP[i]);
    }


}