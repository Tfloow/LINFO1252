#include <stdio.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>
#include <sys/mman.h>
#include "lib_tar.h"
#include <dirent.h>
#include <string.h>

#define BLK_SIZE = 4096

struct stat info;

// POSIX Header

tar_header_t tar_info;
tar_header_t* tar_array;
int num_files;
int stored = 0;

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

void print_tar_header(tar_header_t tar_head){
    printf("______________Tar Header_______________\n");
    printf("Name:\t\t %s \n", tar_head.name);
    printf("Mode:\t\t %ld \n", TAR_INT(tar_head.mode));
    printf("User ID:\t %ld \n", TAR_INT(tar_head.uid));
    printf("Group ID:\t %ld \n", TAR_INT(tar_head.gid));
    printf("Size:\t\t %ld \n", TAR_INT(tar_head.size));

    readable_time(atoi(tar_head.mtime));

    printf("Modified time:\t %s \n", buf);

    printf("Check Sum:\t %ld \n", TAR_INT(tar_head.chksum));
    printf("Typeflag:\t %c \n", tar_head.typeflag);
    printf("Linkname:\t %ld \n", TAR_INT(tar_head.linkname));
    printf("Magic:\t\t %s \n", tar_head.magic);
    printf("Version:\t %ld \n", TAR_INT(tar_head.version));
    printf("Uname:\t\t %s \n", tar_head.uname);
    printf("Gname:\t\t %s \n", tar_head.gname);
    printf("Devmajor:\t %s \n", tar_head.devmajor);
    printf("Devminor:\t %s \n", tar_head.devminor);
    printf("Prefix:\t\t %s \n", tar_head.prefix);
    printf("Padding:\t %s \n", tar_head.padding);
    
    printf("______________End Tar Header_______________\n");
}

// HELPER FUNCTION

/**
 * Convert a 512 bytes buffer to the struct tar_header_t tar_info variable
 * @param read_buf: pointer to the 512 bytes buffer
 * @return nothing but sliced the buffer properly into the global variable tar_info
**/
int put_tar_info(void* read_buf){
    for(int i = 0; i < 512; i++){
        if(i < 100){
            tar_info.name[i] = *((int*) read_buf);
        }else if (i<108)
        {
            tar_info.mode[i-100] = *((int*) read_buf);
        }else if (i<116)
        {
            tar_info.uid[i-108] = *((int*) read_buf);
        }else if (i<124)
        {
            tar_info.gid[i-116] = *((int*) read_buf);
        }else if (i<136)
        {
            tar_info.size[i-124] = *((int*) read_buf);
        }else if (i<148)
        {
            tar_info.mtime[i-136] = *((int*) read_buf);
        }else if (i<156)
        {
            tar_info.chksum[i-148] = *((int*) read_buf);
        }else if (i<157)
        {
            tar_info.typeflag = *((int*) read_buf);
        }else if (i<257)
        {
            tar_info.linkname[i-157] = *((int*) read_buf);
        }else if (i<263)
        {
            tar_info.magic[i-257] = *((int*) read_buf);
        }else if (i<265)
        {
            tar_info.version[i-263] = *((int*) read_buf);
        }else if (i<297)
        {
            tar_info.uname[i-265] = *((int*) read_buf);
        }else if (i<329)
        {
            tar_info.gname[i-297] = *((int*) read_buf);
        }else if (i<337)
        {
            tar_info.devmajor[i-329] = *((int*) read_buf);
        }else if (i<345)
        {
            tar_info.devminor[i-337] = *((int*) read_buf);
        }else if (i<500){
            tar_info.prefix[i-345] = *((int*) read_buf);
        }else{
            tar_info.padding[i-500] = *((int*) read_buf);
        }
        
        read_buf++;
    }

    //printf("Name: %s\n", tar_info.name);

    if(TAR_INT(tar_info.chksum) == 0){
        return -1;
    }
    return 0;
}

int checksum(void* read_buf){
    unsigned int summing = 0;
    char* conv_buf = (char*) read_buf;

    for(int i = 0; i < 148; i++){
        summing += conv_buf[i];
    }
    for(int i = 156; i < 512; i++){
        summing += conv_buf[i];
    }
    summing += 8*32;
    summing %= 65535;

    return summing;
}

/**
 * A simple function that should always be called at the end of each function reading or calling a function
 * that moves the pointer to a file (eg: check_archive, exists, store_header)
 * @param tar_fd: a file descriptor to a tar archive
 * @return 0 if it was successful or 1 if it wasn't. 
 * **/
int reset_fd(int tar_fd){
    if (lseek(tar_fd, 0, SEEK_SET) == -1) {
        perror("Error resetting file position");
        close(tar_fd);
        return 1;
    }

    return 0;
}

/**
 * A simple function that tells if a path is the sub-folder of one other
 * @param root: the path that points to the root
 * @param check_folder: the path where we want to check if it's a simple subfolder
 * @return: -1 if it's deeper than a simple subfolder
 *          0 if it's indeed a subfolder and no more
 * **/
int is_sub_sub_folder(char* root, char* check_folder){
    int max_depth = 1;
    for(int j = strlen(root); j < strlen(check_folder); j++){
        if(max_depth <= 0){
            printf("TOO DEEP\n");
            return -1;
        }else if(check_folder[j] == '/'){
            max_depth--;
        }
    }

    return 0;
}

/**
 * A function that check if a certain directory is part of another directory (no matter how deep it is)
 * @param root: root folder to check from
 * @param check: the file or directory if it's part of the root directory
 * @return: -1 if it's not part of the root directory
 *          0 if it's part of the root directory
 * **/
int is_part_sub_folder(char* root, char* check){
    if(strlen(check) < strlen(root)){
        return -1;
    }

    for(int i = 0; i < strlen(root); i++){
        if(root[i] != check[i]){
            return -1;
        }
    }

    return 0;
}

/**
 * A simple function that takes the linkname of a symlink directory and fuse it to have a proper
 * path of the tar archive
 * @param linkTo: a linkname from a symlink directory
 * @return: return the fused name 
 * **/
char* fuse_sub_symlink(char* linkTo){
    char* res = (char*) malloc(sizeof(char) * (strlen(tar_array[0].name) + strlen(linkTo)+2));
    for(int i = 0; i < strlen(tar_array[0].name) ; i++){
        res[i] = tar_array[0].name[i];
    }
    for(int i = 0; i < strlen(linkTo) ; i++){
        res[strlen(tar_array[0].name) + i] = linkTo[i];
    }
    res[strlen(tar_array[0].name) + strlen(linkTo)] = '/'; 
    res[strlen(tar_array[0].name) + strlen(linkTo) + 1] = '\0'; 
    return res;
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
        return -4;
    } 

    //print_stat(tar_fd);
    // Read the whole tar file
    void* read_buf = malloc(info.st_size);
    read(tar_fd,read_buf,info.st_size);
    
    // Flag if there is multiple files
    int otherFiles = 1;
    int skip;
    int tmp = 0;

    // 44032 for archive.tar

    while(otherFiles){
        if(put_tar_info(read_buf) == -1){
            printf("bye: %d\n", tmp);
            break;
        }
        //print_tar_header(tar_info);

        if(TAR_INT(tar_info.size) == 0){
            skip = 1;
        }else{
            skip = TAR_INT(tar_info.size)/512 + 2;
            //printf("to skip %d\n", skip);
        }

        // Check for the magic string
        int magicCMP = strcmp(TMAGIC, tar_info.magic);
        //printf("magic %s and comp %d and length of %ld\n", tar_info.magic,magicCMP, strlen(tar_info.magic));
        if(magicCMP || strlen(tar_info.magic)+1 != TMAGLEN){ // +1 for the null Character
            return -1;
        }

        // Check for the version
        for(int i = 0; i < 2; i++){
            if(tar_info.version[i] != '0'){
                return -2;
            }
        }

        // Check for the checksum
        if(checksum(read_buf) != TAR_INT(tar_info.chksum)){
            return -3;
        }

        read_buf += skip*512;
        //printf("skipped: %d\n", skip*512);
        tmp++;

    }

    num_files = tmp;
    reset_fd(tar_fd);

    return tmp;
}


void print_tar_array(){
    printf("________TAR ARRAY________\n");
    for(int i = 0; i < num_files; i++){
        print_tar_header(tar_array[i]);
    }
    printf("________END TAR ARRAY________\n");

}


// TO STORE HEADER TAR INFO
int store_header(int tar_fd){
    if(stored != 0){
        return 0;
    }

    if(check_archive(tar_fd) < 0){
        printf("NOT A CORRECT HEADER\n");
        return -1;
    }
    printf("Checking\n");

    tar_array = (tar_header_t*) malloc(sizeof(tar_header_t) * num_files);

    // Read the whole tar file
    void* read_buf = malloc(info.st_size);
    read(tar_fd,read_buf,info.st_size);
    int skip = 0;

    for(int i = 0; i < num_files; i++){
        put_tar_info(read_buf);
        
        if(TAR_INT(tar_info.size) == 0){
            skip = 1;
        }else{
            skip = TAR_INT(tar_info.size)/512 + 2;
            //printf("to skip %d\n", skip);
        }

        memcpy( &(tar_array[i]), &tar_info, sizeof(tar_header_t));
        
        read_buf += skip*512;
    }

    reset_fd(tar_fd);
    stored = 1;
    //print_tar_array();
    return 0;
}

// Combining all is
/**
 * Combining all the exists, is_dir, is_file, ... function into one and only function
 * @param   tar_fd:   a file descriptor to a valid tar archive
 * @param   path:     a path to a file or directory in the archive
 * @param   typeflag: a flag to tell what kind of thing we are looking for. If we don't care about a
 *                    type we must set typeflag to -1 in order to do so
 * @return return 0 if it didn't find anything or encountered an issue and index+1 if it did find it
**/
int is(int tar_fd, char* path, int typeflag){
    if(store_header(tar_fd) < 0){
        printf("The archive is not valid\n");
        return 0;
    }

    for(int i = 0; i < num_files; i++){ // iterate over the amount of file in the buffer
        if(typeflag == -1){
            if(strcmp(tar_array[i].name, path) == 0){
                printf("File does exist in archive\n");
                return i+1;
            }
        }else{
            if(strcmp(tar_array[i].name, path) == 0 && tar_array[i].typeflag == typeflag){
                printf("File does exist in archive\n");
                return i+1;
            }
        }
    }
    printf("No such file in archive\n");

    reset_fd(tar_fd);
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
    return is(tar_fd, path, -1);
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
    return is(tar_fd, path, DIRTYPE);    
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
    return is(tar_fd, path, REGTYPE);
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
    return is(tar_fd, path, SYMTYPE);
}

char* easy_fuse(int index){
    char* link = tar_array[index-1].linkname;
    int depth = 0;

    for(int i = 0; i < strlen(link); i++){
        if(link[i] == '/'){
            depth++;
        }
    }

    static char new_path[100];
    int root = 0;
    char* from = tar_array[index-1].name;
    for(int i = 0; i < strlen(from); i++){
        if(from[i] == '/'){
            root++;
        }
    }

    int go_back = root - depth;

    printf("  from %s and linked to %s and depth %d\n", from, link, go_back);


    int i = 0;
    int j = 0;
    for(; i < strlen(from); i++){
        if(go_back <= 0){
            break;
        }

        printf("%c ", from[i]);

        new_path[i] = from[i];
        if(from[i] == '/'){
            go_back--;
        }
        j++;

    }
    printf("\n %d", i-j);

    for( ; i-j < strlen(link); i++){
        new_path[i] = link[i-j];

        printf("%d %c ",i-j, link[i-j]);

    }
    printf("\n");

    new_path[i] = '\0';

    printf(" RES easy fuse %s\n", new_path);

    return new_path;
}

int resolve_sym(int tar_fd, char* path){
    int where = is_file(tar_fd, path);
    if(where == 0){
        where = is_dir(tar_fd, path);
        if(where == 0){
            int sym = is_symlink(tar_fd, path);
            while( sym != 0 ){
                printf("    aaaaaaah\n");
                where = is_dir(tar_fd, easy_fuse(sym));
                sym = is_symlink(tar_fd, easy_fuse(sym));
            }
            printf("FINI WHILE\n");

        }
    }

    return where;
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
    entries = (char**) malloc(*no_entries * sizeof(char*));
    for(int i = 0; i < *no_entries; i++){
        entries[i] = (char*) malloc(100 * sizeof(char));
    }

    int where_dir = is_dir(tar_fd, path);

    if(where_dir == 0){
        where_dir = is_symlink(tar_fd, path);
        if(where_dir == 0){
            *no_entries = 0;
            return 0;
        }
        where_dir = resolve_sym(tar_fd, path);
    }

    printf("   where dir %d with %s\n", where_dir, tar_array[where_dir-1].name);

    if(tar_array[where_dir].typeflag != REGTYPE || where_dir == 0){
        *no_entries = 0;
        return 0;
    }

    int i = where_dir-1;
    char* root = tar_array[i].name;

    printf("starting %d\n", i);

    int actual_num_file = 0;

    for(int j = i+1; j < num_files; j++){
        // To stay in the same directory to avoid useless search
        if(is_part_sub_folder(root, tar_array[j].name) != 0){
            printf("CIAOOO\n");
            break;
        }

        printf("String %s from %s\n", tar_array[j].name, root);


        if(is_sub_sub_folder(root, tar_array[j].name) == 0){
            printf("added \n");
            entries[actual_num_file] = tar_array[j].name;
            actual_num_file++;
        }

    }

    *no_entries = actual_num_file;
    return 1;
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



size_t pad(size_t size_file){

    int mod = size_file % 512;
    if(mod == 0){return size_file;}
    int div = size_file/512;
    return 512*(div + 1);
}

ssize_t read_file(int tar_fd, char *path, size_t offset, uint8_t *dest, size_t *len) {

    size_t real_offset = 0;


    int exist = is_file(tar_fd, path);
    if(exist == 0){
        exist =  is_symlink(tar_fd, path);
        if(exist == 0){
            return -1;
        }
        exist = is_file(tar_fd, tar_array[exist-1].linkname);
        if(exist == 0){
            return -1;
        }
    }

    int size = TAR_INT(tar_array[exist-1].size);
    if(offset > size){return -2;}

    for(int i = 0; i< num_files; i++){
        if(strcmp(tar_array[i].name, path) == 0){break;}

        real_offset += pad(TAR_INT(tar_array[i].size)) + 512;
    }

    

    //in case the buffer is too big:
    if(*len > size - offset){*len = size - offset;}


    //compute the remaining amount of bytes
    ssize_t rem_bytes = size - offset - *len;
    //the whole file is read and dest contains too much space:
    if(rem_bytes < 0){rem_bytes = 0;}


    /// Set offset:///

    int err_seek = lseek(tar_fd, real_offset + offset + 512, SEEK_SET);
    if(err_seek == -1){printf("Error during lseek\n");}


   int palu = read(tar_fd, dest, *len);
   if(palu == -1){printf("Error during lecture time\n");}

   int reset = lseek(tar_fd, 0, SEEK_SET);
   if(reset == -1){printf("Error during pointer rester\n");}


    return rem_bytes;
}