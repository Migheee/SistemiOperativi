

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>




int main(){
    int st;
    int fd[2];
    int p, d, somma=0;
    if((d=pipe(fd))==-1){
        printf("errore creazione pipe\n");
        exit(1);
    }
    if((p=fork())==0){ //figlio lettore
        close(fd[1]);
        while(read(fd[0], &d, sizeof(int))>0){
            printf("processo lettore ha letto %d\n", d);
            somma+=d;
        }
        printf("il lettore ha terminato la lettura, la somma è %d\n", somma);
        exit(54);
    }

    else{
        if((p=fork())==0){ //figlio scrittore
            close(fd[0]);
            for(int i=0; i<10; i++){
                write(fd[1], &i, sizeof(int));
                printf("processo scr con pid %d scrive %d\n", getpid(), i);
            }
            printf("il scrittore ha terminato la scrittura\n");
            exit(50);

        }
        else{
            close (fd[0]);
            close (fd[1]);
            int fid;
            while ((fid = wait(&st)) != -1) {
                if (WIFEXITED(st)) { 
                    printf("\nFiglio con pid = %d terminato OK con exit code = %d\n", fid, WEXITSTATUS(st)); //WEXITSTATUS:exitcode
                } else if (WIFSIGNALED(st)) {
                    printf("\nFiglio con pid = %d terminato dal segnale n: %d\n", fid, WTERMSIG(st)); //WTERMSIG: segnale di terminazione
                } else if (WIFSTOPPED(st)) {
                    printf("\nFiglio con pid = %d stoppato dal segnale n: %d\n", fid, WSTOPSIG(st)); //WSTOPSIG: segnale di stop
                }
            }
        }
    }
}

