#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>

int main(int argc, char* argv[]){
    char content[]= "#include <stdio.h>\n int main( .... ){\n ....\n}";
    mode_t mode = S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH; //stessa cosa di 755
    if(argc==1){
        printf("Non e' stato inserito il nome del file\n");
        exit(-1);
    }
    else{
        int fd = open(argv[1], O_CREAT | O_WRONLY | O_TRUNC, mode); //nicola drago rulez
        if(fd == -1){
            printf("C'è stato un errore nella creazoine del file\n");
            exit(-1);
        }
        int result = write(fd, content, strlen(content));
        if (result==-1){
            printf("La scrittura non è anadata a buon fine\n");
        }
        printf("Scrittura completata\n");
        close(fd);
    }
 
    return 0;

}