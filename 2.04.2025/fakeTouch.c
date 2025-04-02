/*
    Hint: leggere e scrivere un byte
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char* argv[]){
    char buf[1];
    if(argc==1){
        printf("Non e' stato inserito il nome del file\n");
        exit(-1);
    }
    else{
        int fd = open(argv[1], O_RDWR , 0755);
        if(fd==-1){
            printf("Errore nell' aperutra del file");
        }
        int letti = read(fd, buf ,1 );
        if(letti == -1){
            printf("Errore nell'esecuzione della read\n");
        }
        if(write ( fd , buf , letti ) != letti){
            printf("Errore nell'esecuzione della write\n");
            exit(-1);
        }
        printf("Touch completato \n");
        close(fd);
        return 0;

    }
}