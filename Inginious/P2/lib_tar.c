#include <stdio.h>
#include <sys/stat.h>
#include <time.h>

#include "lib_tar.h"
#include <dirent.h>
#include <string.h> 

struct stat info;

// POSIX Header

tar_header_t tar_info;

// For readable date
time_t     now;
struct tm  ts;
char       buf[80];

// DEBUG FUNCTION

/**
    Readable time. Convert time from int to a readable output
    @param since_epoch: an int with the elapsed time since epoch
    @return nothing put but a string inside the global variable buf
**/ 
void readable_time(int since_epoch){
    now = (time_t) info.st_atime;
    ts = *localtime(&now);
    strftime(buf, sizeof(buf), "%a %Y-%m-%d %H:%M:%S %Z", &ts);
}

void print_stat(int tar_fd){
    if(fstat(tar_fd, &info) == -1){
        perror("fstat(tar_file)");
        return;
    } 



    now = (time_t) 1702458988;

    // Format time, "ddd yyyy-mm-dd hh:mm:ss zzz"
    printf("%s\n", buf);

    printf("______________Info_______________\n");
    printf("Permissions:\t\t %d \n", info.st_mode);
    printf("Device on:\t\t %ld \n", info.st_dev);
    printf("User ID:\t\t %d \n", info.st_uid);
    printf("Group ID:\t\t %d \n", info.st_gid);

    readable_time(info.st_atime);

    printf("Accessed time:\t\t %s \n", buf);

    readable_time(info.st_atime);

    printf("Permission change time:\t %s \n", buf);

    readable_time(info.st_atime);

    printf("Last modification time:\t %s \n", buf);
    printf("Amount of link:\t\t %ld\n", info.st_nlink);
    printf("______________Crucial_______________\n");
    printf("Size:\t\t\t %ld \n", info.st_size);
    printf("Block Size:\t\t %ld \n", info.st_blksize);
    printf("Block:\t\t\t %ld \n", info.st_blocks);
    printf("______________End Info_______________\n");


}

void print_tar_header(){
    printf("______________Tar Header_______________\n");
    printf("Name:\t\t %s \n", tar_info.name);
    printf("Mode:\t\t %s \n", tar_info.mode);
    printf("User ID:\t\t %s \n", tar_info.uid);
    printf("Group ID:\t\t %s \n", tar_info.gid);
    printf("Size:\t\t %s \n", tar_info.size);

    readable_time(atoi(tar_info.mtime));

    printf("Modified time:\t %s \n", buf);

    printf("Check Sum:\t\t %s \n", tar_info.chksum);
    printf("______________End Tar Header_______________\n");
}

// HELPER FUNCTION

/**
 * Convert a 512 bytes buffer to the struct tar_header_t tar_info variable
 * @param read_buf: pointer to the 512 bytes buffer
 * @return nothing but sliced the buffer properly into the global variable tar_info
**/
void put_tar_info(void* read_buf){
    for(int i = 0; i < 512; i++){
        if(i < 100){
            tar_info.name[i] = *((char*) read_buf);
        }else if (i<108)
        {
            tar_info.mode[i] = *((char*) read_buf);
        }else if (i<116)
        {
            tar_info.uid[i] = *((char*) read_buf);
        }else if (i<124)
        {
            tar_info.gid[i] = *((char*) read_buf);
        }else if (i<136)
        {
            tar_info.size[i] = *((char*) read_buf);
        }else if (i<148)
        {
            tar_info.mtime[i] = *((char*) read_buf);
        }else if (i<156)
        {
            tar_info.chksum[i] = *((char*) read_buf);
        }else if (i<157)
        {
            tar_info.typeflag = *((char*) read_buf);
        }else if (i<257)
        {
            tar_info.linkname[i] = *((char*) read_buf);
        }else if (i<263)
        {
            tar_info.magic[i] = *((char*) read_buf);
        }else if (i<265)
        {
            tar_info.version[i] = *((char*) read_buf);
        }else if (i<297)
        {
            tar_info.uname[i] = *((char*) read_buf);
        }else if (i<329)
        {
            tar_info.gname[i] = *((char*) read_buf);
        }else if (i<337)
        {
            tar_info.devmajor[i] = *((char*) read_buf);
        }else if (i<345)
        {
            tar_info.devminor[i] = *((char*) read_buf);
        }else if (i<500){
            tar_info.prefix[i] = *((char*) read_buf);
        }else{
            tar_info.padding[i] = *((char*) read_buf);
        }
        
        read_buf++;
    }
}

/**
 * Checks whether the archive is valid.
 *
 * Each non-null header of a valid archive has:
 *  - a magic value of "ustar" and a null,
 *  - a version value of "00" and no null,
 *  - a correct checksum
 *
 * @param tar_fd A file descriptor pointing to the start of a file supposed to contain a tar archive.
 *
 * @return a zero or positive value if the archive is valid, representing the number of non-null headers in the archive,
 *         -1 if the archive contains a header with an invalid magic value,
 *         -2 if the archive contains a header with an invalid version value,
 *         -3 if the archive contains a header with an invalid checksum value
 */
int check_archive(int tar_fd) {
    if(fstat(tar_fd, &info) == -1){
        perror("fstat(tar_file)");
        return -1;} 

    print_stat(tar_fd);

    void* read_buf = malloc(info.st_blksize*8);

    read(tar_fd,read_buf,info.st_blksize);
    printf("buffer: %s\n", (char*) read_buf);

    put_tar_info(read_buf);
    print_tar_header();


    //void* buffer = malloc(sizeof())
    return 0;
}


/**
 * Checks whether an entry exists in the archive.
 *
 * @param tar_fd A file descriptor pointing to the start of a valid tar archive file.
 * @param path A path to an entry in the archive.
 *
 * @return zero if no entry at the given path exists in the archive,
 *         any other value otherwise.
 */
int exists(int tar_fd, char *path) {

/*
    int check = check_archive(tar_fd);
    if(check < 0){
        printf("The archive is not valid\n");
        return 0;}

*/
    char *buffer = (char *) malloc(1*sizeof(char)+1);

    

    int size_path = strlen(path);

    printf("size of path: %d\n", size_path);


    for(int i = 0; i < size_path; i++){
        read(tar_fd, (void *) (buffer),1);
        if(*(path + i) != *buffer){
            printf("the entry does not exist\n");
            return 0;
        }
        
    }


    

    return 1;
}

/**
 * Checks whether an entry exists in the archive and is a directory.
 *
 * @param tar_fd A file descriptor pointing to the start of a valid tar archive file.
 * @param path A path to an entry in the archive.
 *
 * @return zero if no entry at the given path exists in the archive or the entry is not a directory,
 *         any other value otherwise.
 */
int is_dir(int tar_fd, char *path) {

    int exist = exists(tar_fd, path);
    if(exist == 0){
        printf("The entry does not exist\n");
    return 0;}

    DIR* dir = opendir(path);
    if(dir == NULL){
        printf("The entry is not a directory");
    return 0;}
    

    return 1;
}

/**
 * Checks whether an entry exists in the archive and is a file.
 *
 * @param tar_fd A file descriptor pointing to the start of a valid tar archive file.
 * @param path A path to an entry in the archive.
 *
 * @return zero if no entry at the given path exists in the archive or the entry is not a file,
 *         any other value otherwise.
 */
int is_file(int tar_fd, char *path) {

    int check = check_archive(tar_fd);
    if(check < 0){
        printf("The archive is not valid\n");
        return 0;}

    return 0;
}

/**
 * Checks whether an entry exists in the archive and is a symlink.
 *
 * @param tar_fd A file descriptor pointing to the start of a valid tar archive file.
 * @param path A path to an entry in the archive.
 * @return zero if no entry at the given path exists in the archive or the entry is not symlink,
 *         any other value otherwise.
 */
int is_symlink(int tar_fd, char *path) {

    int check = check_archive(tar_fd);
    if(check < 0){
        printf("The archive is not valid\n");
        return 0;}

    return 0;
}


/**
 * Lists the entries at a given path in the archive.
 * list() does not recurse into the directories listed at the given path.
 *
 * Example:
 *  dir/          list(..., "dir/", ...) lists "dir/a", "dir/b", "dir/c/" and "dir/e/"
 *   ├── a
 *   ├── b
 *   ├── c/
 *   │   └── d
 *   └── e/
 *
 * @param tar_fd A file descriptor pointing to the start of a valid tar archive file.
 * @param path A path to an entry in the archive. If the entry is a symlink, it must be resolved to its linked-to entry.
 * @param entries An array of char arrays, each one is long enough to contain a tar entry path.
 * @param no_entries An in-out argument.
 *                   The caller set it to the number of entries in `entries`.
 *                   The callee set it to the number of entries listed.
 *
 * @return zero if no directory at the given path exists in the archive,
 *         any other value otherwise.
 */
int list(int tar_fd, char *path, char **entries, size_t *no_entries) {

    int check = check_archive(tar_fd);
    if(check < 0){
        printf("The archive is not valid\n");
        return 0;}

    return 0;
}

/**
 * Reads a file at a given path in the archive.
 *
 * @param tar_fd A file descriptor pointing to the start of a valid tar archive file.
 * @param path A path to an entry in the archive to read from.  If the entry is a symlink, it must be resolved to its linked-to entry.
 * @param offset An offset in the file from which to start reading from, zero indicates the start of the file.
 * @param dest A destination buffer to read the given file into.
 * @param len An in-out argument.
 *            The caller set it to the size of dest.
 *            The callee set it to the number of bytes written to dest.
 *
 * @return -1 if no entry at the given path exists in the archive or the entry is not a file,
 *         -2 if the offset is outside the file total length,
 *         zero if the file was read in its entirety into the destination buffer,
 *         a positive value if the file was partially read, representing the remaining bytes left to be read to reach
 *         the end of the file.
 *
 */
ssize_t read_file(int tar_fd, char *path, size_t offset, uint8_t *dest, size_t *len) {
        int check = check_archive(tar_fd);
    if(check < 0){
        printf("The archive is not valid\n");
        return -1;}
    return 0;
}