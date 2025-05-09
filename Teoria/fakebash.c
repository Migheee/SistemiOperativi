#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>


char comm[10];
int main(){
    int pid, st;
    while(1){
        scanf("%s", comm);
        if(!strcmp(comm, "exit")==0){
            if(pid=fork()==0){
                execlp(comm, comm, NULL);
                perror("Comando Fallito");
                _exit(0x15); //Non svuota il buffer di stdio
            }
            else{
                wait(&st);
            }
        }
        else{
            exit(0);
        }
    }
}