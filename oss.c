#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include <sys/types.h>

int main(int argc,  char* argv[]){
    int Opt;
    int TotalChildren = -1;
    int MaxProcess = -1;
    char* Iterations = NULL;
    pid_t isParent = -1;
    int currentChildren =  0;
    int AllChildren = 0;
    int status;


    while((Opt = getopt(argc, argv, "n:s:t:h")) != -1){
        // opt arguments -n, -s, -t
        switch(Opt){
            case 'h':
                printf("./oss [-n proc] [-s simul] [-t iter] [-h]\n");
                printf("proc: number of processes to run\n");
                printf("simul: number of processes to run at a time\n");
                printf("iter: number of iterations for user to run\n");
                return 0;
            case 'n':
                TotalChildren = atoi(optarg);
                printf("Total Children: %d\n", TotalChildren);
                break;
            case 's':
                MaxProcess = atoi(optarg);
                printf("Max Process: %d\n", MaxProcess);
                break;
            case 't':
                Iterations = optarg;
                printf("Iterations: %s\n", Iterations);
                break;
            default:
                printf("Option ?\n");
                break;
        }

        if(TotalChildren == -1 || MaxProcess == -1 || Iterations == NULL){
            printf("Please enter all the arguments\n");
            return 1;
        }
    }
    // AllChildren is number of chilren run, TotalChildren is the number specified.
    while(AllChildren < TotalChildren){
        
        isParent = fork();
        if(isParent == 0){
            execvp("./user", Iterations);
            wait(&status);
            return 0;
        }
        else if (isParent == 1){

        }
        else{
            printf("Error: Failed to launch child process\n");
            return 1;
        }
    }

     printf("Hello, World!\n");
    return 0;
}