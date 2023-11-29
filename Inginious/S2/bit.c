#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

uint8_t get_3_leftmost_bits(uint32_t x) {
    uint32_t comp = 0b11100000000000000000000000000000;

    uint8_t res = (x & comp) >> 29;

    return res;
}

uint8_t get_4_rightmost_bits(uint32_t x) {
    uint32_t comp = 0b00000000000000000000000000001111;

    uint8_t res = (x & comp) ;

    return res;
}

uint8_t nbits(uint32_t n) {
    uint32_t mask = 0b0000000000000000000000000000001;
    uint8_t count = 0;

    for(int i = 0; i < 32; i++){
        if((mask & n) != 0) {
            count++;
        }
        mask = mask << 1;
    }

    return count;
}

uint32_t reset_highestorder_strong_bit(uint32_t x) {
    uint32_t mask = 0b0000000000000000000000000000001;
    uint32_t highest = 0;

    for(int i = 0; i < 32; i++){
        if((mask & x) != 0) {
            highest = mask;
        }
        mask = mask << 1;
    }

    x-=highest;

    return x;
}

uint32_t cycle_bits(uint32_t x, uint8_t n) {
    uint32_t old = x;

    x = x << n;
    x = x + (old>>(32-n));

    return x;
}

unsigned int stexp() { 
    static int n = 0;
    int res = 1;

    for(int i = 0; i < n; i++){
        res*=2;
    }

    if(n >= 13){
        n = 0;
        res = 1;
    }
    n++;
    return res;
}


int main(int argc, char* argv[]){

    uint32_t x = 0b11100000000000000000000000000111;
    uint8_t y = 0b10101010;

    printf("%d\n",cycle_bits(y, 2));

    for(int i = 0; i < 30; i++){
        printf("%d \n", stexp());
    }

    return EXIT_SUCCESS;
}