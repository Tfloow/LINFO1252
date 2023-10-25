#include <stdio.h>
#include <string.h>
#include <sys/time.h>

int main(){
    FILE *file = fopen("appels_systemes.txt", "w+");
    if(file == NULL){
        printf("Erreur d'ouverture du fichier\n");
        return 1;
    }
    struct timeval tv;
    gettimeofday(&tv, NULL);
    char buffer[100];
    int len = sprintf(buffer, "Le nombre de secondes ecoulees depuis le 1er janvier 1970 est : %ld.%06ld", tv.tv_sec, tv.tv_usec);
    int err = fwrite(buffer, 1, len, file);
    if(err != len){
        printf("Erreur d'ecriture dans le fichier\n");
        fclose(file);
        return 1;
    }
    printf("Ecrit dans le fichier:%d octets\n", len);
    
    fseek(file, 0, SEEK_SET);
    err = fread(buffer, 1, len, file);
    if(err < 0){
        printf("Erreur de lecture dans le fichier\n");
        fclose(file);
        return 1;
    }
    printf("Lu dans le fichier:%d octets\n", err);
    fclose(file); 
}