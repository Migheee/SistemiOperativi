#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

int main(void) {
    int pid, fid, st;


    if ((pid = fork()) == 0) {
        printf("\nSono primo figlio con pid = %d\n", getpid());
        exit(40);
    } else {

        if ((pid = fork()) == 0) {
            printf("\nSono sec figlio con pid = %d\n", getpid());
            alarm(5);      
            pause(); //se non ci fosse pause(), probabilmente faceva in tempo a fare la exit...
            exit(50);
        } else {
            while ((fid = wait(&st)) != -1) {
                if (WIFEXITED(st)) { //usando queste macro si interroga st. negli ultimi 2 byte a dx si vanno a vedere i segnali e l'exit code
                    printf("\nFiglio con pid = %d terminato OK con exit code = %d\n", fid, WEXITSTATUS(st)); //WEXITSTATUS:exitcode
                } else if (WIFSIGNALED(st)) {
                    printf("\nFiglio con pid = %d terminato dal segnale n: %d\n", fid, WTERMSIG(st)); //WTERMSIG: segnale di terminazione
                } else if (WIFSTOPPED(st)) {
                    printf("\nFiglio con pid = %d stoppato dal segnale n: %d\n", fid, WSTOPSIG(st)); //WSTOPSIG: segnale di stop
                }
            }
            printf("\n... Tutti i figli sono terminati\n");
        }
    }

    return 0;
}
