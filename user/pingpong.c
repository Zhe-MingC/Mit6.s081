// Simple pingpong, 

#include "kernel/types.h"
#include "user/user.h"

int main(int argc, char *argv[]){
    int p[2];
    pipe(p);
    char buffer[20];
    if(fork() == 0){
        // close(p[1]);
        read(p[0], buffer, sizeof buffer);
        printf("%d: received %s\n", getpid(), buffer);
        write(p[1], "pong", 5);
        exit(0);
    }else{
        // close(p[0]);
        write(p[1], "ping", 5);
        wait((int *)0);
        read(p[0], buffer, sizeof buffer);
        printf("%d: received %s\n", getpid(), buffer);
    }   
    exit(0);
}