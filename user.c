#include <stdio.h>
#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>

int main(int argc, char* argv[]){
    for(int i = 0; i < atoi(argv[1]); i++){
        printf("USER PID: %d PPID: %d Iteration: %d before sleeping\n", getpid(), getppid(), i);

        sleep(1);

        printf("USER PID: %d PPID: %d Iteration: %d after sleeping\n", getpid(), getppid(), i);
    }
    return 0;
}