#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#include "lib_tar.h"

/**
 * You are free to use this file to write tests for your implementation
 */

void debug_dump(const uint8_t *bytes, size_t len) {
    for (int i = 0; i < len;) {
        printf("%04x:  ", (int) i);

        for (int j = 0; j < 16 && i + j < len; j++) {
            printf("%02x ", bytes[i + j]);
        }
        printf("\t");
        for (int j = 0; j < 16 && i < len; j++, i++) {
            printf("%c ", bytes[i]);
        }
        printf("\n");
    }
}

int main(int argc, char **argv) {
    if (argc < 2) {
        printf("Usage: %s tar_file\n", argv[0]);
        return -1;
    }

    int fd = open(argv[1] , O_RDONLY);
    if (fd == -1) {
        perror("open(tar_file)");
        return -1;
    }

    printf("____CHECK ARCHIVE TEST____\n");
    int ret = check_archive(fd);
    printf("check_archive returned %d\n", ret);

    store_header(fd);
    printf("____EXIST TEST____\n");
    int exist = exists(fd, "tarInfo.txt");

    if(exist == 0){printf("Rien\n");}

    printf("____IS DIR TEST____\n");
    int is_directory = is_dir(fd, "hard/");

    if(is_directory == 0){printf("Rien\n");}

    printf("____LIST TEST____\n");
    size_t amount = 4;
    char** entries = (char**) malloc(amount * sizeof(char*));
    for(int i = 0; i < amount; i++){
        entries[i] = (char*) malloc(100 * sizeof(char));
    }

    int listing = list(fd, "hard/subSym", entries, &amount);

    printf("amount : %ld\n", amount);
    if(strcmp(entries[0], "hard/sub/empty.txt") == 0){
        printf("Success !\n");
    }else{
        printf("Back to the drawing board\n");
    }

    if(listing == 0){printf("Rien\n");}

    return 0;

    printf("____READ FILE TEST____\n");
    size_t len = 100;
    uint8_t* dest = (uint8_t*) malloc(sizeof(uint8_t) * len);

    int reading = read_file(fd, "aled.txt",0, dest, &len);
    if(reading == 0){
        printf("On est good\n");
    }

    return 0;
}