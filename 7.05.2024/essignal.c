#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void cc_user(int);

int main(){
    signal(SIGINT, cc_user);
    while(1){
        printf("prova ad interrompermi\n");
        sleep(5);
    }
}

void cc_user(int x){
    fprintf(stderr, "\nsegnale %d e non mi interrompi!!\n",x);
    signal(SIGINT, SIG_DFL);
}