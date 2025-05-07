/*
fare in modo che dopo sigchld, il gestore dei segnali facci la wait e dezombifichi il figlio
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

void signal_manager(int);

int main(){
    pid_t pid;
    signal(SIGCHLD, signal_manager);
    if((pid=fork())==0){
        alarm(5);
        pause();
        exit(40);
    }
    else{
        while(1);
    }
}

void signal_manager(int){
    pid_t dead;
    int st;
    while ((dead = wait(&st)) != -1)
    {
        if (WIFSIGNALED(st))
        printf("figlio con pid %d terminato con segnale = %d \n " ,dead,  WTERMSIG(st));
    }
    exit(0);
}
