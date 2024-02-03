#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>

int main(int argc,  char* argv[]){
    int Opt;
    int TotalChildren = -1;
    int MaxProcess = -1;
    int Iterations = -1;


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
                Iterations = atoi(optarg);
                printf("Iterations: %d\n", Iterations);
                break;
            default:
                printf("Option ?\n");
                break;
        }

        if(TotalChildren == -1 || MaxProcess == -1 || Iterations == -1){
            printf("Please enter all the arguments\n");
            return 1;
        }
    }

     printf("Hello, World!\n");
    return 0;
}