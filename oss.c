/*
    Ben Cradick
    CS4760
    Project 1
    Due: 02/07/2024    
*/

#include <stdio.h>
#include <unistd.h>
#include <getopt.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <bits/waitflags.h>
#include <errno.h>
#include <string.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <signal.h>
#include <bits/signum-generic.h>

#define BILLION 1000000000
#define MILLION 1000000

struct ProcessControlBlock
{
    int occupied;     
    pid_t pid;        
    unsigned long nanoSeconds;    
};
struct ProcessControlBlock ProcessTable[20];

void killChildren(){
    for(int i = 0; i < 20; i++){
        if(ProcessTable[i].occupied == 1){
            kill(ProcessTable[i].pid, SIGKILL);
        }
    }
}

int main(int argc,  char* argv[]){
    int Opt;
    int TotalChildren = -1;
    int MaxProcess = -1;
    int MaxRuntime = NULL;
    int launchInterval = 0;
    pid_t IsParent = -1;
    pid_t ChildExited = -1;
    int CurrentChildren =  0;
    int AllChildren = 0;
    int status;

    unsigned long value = 0;
    unsigned long increment = 1000000;
    

    const int SIZE = 8;
    
    const char* NanoSeconds = "NanoSeconds";


    //FD is short fore file descriptor

    int NanoSecondSharedMemoryFD;
    void* NanoSecondSharedMemoryPointer;

    


    NanoSecondSharedMemoryFD = shm_open(NanoSeconds, O_CREAT | O_RDWR, 0666);


    ftruncate(NanoSecondSharedMemoryFD, SIZE);

    


    NanoSecondSharedMemoryPointer = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, NanoSecondSharedMemoryFD, 0);

    signal(SIGALRM, (void (*)(int))killChildren);
    alarm(1);

    while((Opt = getopt(argc, argv, "n:s:t:h:i")) != -1){
        // opt arguments -n, -s, -t
        switch(Opt){
            case 'h':
                printf("./oss [-n proc] [-s simul] [-t timelimitForChildren] [-i intervalInMSToLaunchChildren] [-h]\n");
                printf("proc: number of processes to run\n");
                printf("simul: number of processes to run at a time\n");
                printf("iter: time limit for child\n");
                printf("intervalInMsToLaunchChildren: iterval between process launches in ms\n");
                return 0;
            case 'n':
                TotalChildren = atoi(optarg);
                // printf("Total Children: %d\n", TotalChildren);
                break;
            case 's':
                MaxProcess = atoi(optarg);
                if(MaxProcess >= 20){
                    MaxProcess = 20;
                }
                // printf("Max Process: %d\n", MaxProcess);
                break;
            case 't':
                MaxRuntime = atoi(optarg);
                // printf("Iterations: %s\n", Iterations);
                break;
            case 'i':
                launchInterval = atoi(optarg);
                break;
            default:
                printf("Option ?\n");
                break;
        }

        
    }
    if(TotalChildren == -1 || MaxProcess == -1 || MaxRuntime == NULL){
            printf("Missing argument, assigning obnoxious default values, see the help menu.\n");
            printf("./oss [-n proc] [-s simul] [-t iter] [-h]\n");
            printf("proc: number of processes to run\n");
            printf("simul: number of processes to run at a time\n");
            printf("iter: number of iterations for user to run\n");

            if(TotalChildren == -1){
                TotalChildren = 19;
            }
            if(MaxProcess == -1){
                MaxProcess = 10;
            }
            if(MaxRuntime == NULL){
                MaxRuntime = 7;
            }
            
        }


    // AllChildren is number of chilren run, TotalChildren is the number specified.
    while(AllChildren < TotalChildren){

        memcpy(NanoSecondSharedMemoryPointer, &value, sizeof(value));
        value += increment;

        //check for finished children without hanging
        ChildExited = waitpid(-1, &status, WNOHANG);
        if(ChildExited > 0){
            CurrentChildren--;
            // for each element in the process table check to see if the pid matches the pid of the child that just exited
            for(int i = 0; i < 20; i++){
                if(ProcessTable[i].pid == ChildExited){
                    ProcessTable[i].occupied = 0;
                    ProcessTable[i].pid = 0;
                    ProcessTable[i].nanoSeconds = 0;
                    break;
                }
            }
        }

        //If the number of active children is equal to the number of max processes
        // the program will hang until a child process is finished
        else if(CurrentChildren == MaxProcess){
            ChildExited = waitpid(-1, &status, 0);
            if(ChildExited < 0){
                printf("Error: Failed to wait for child process\nError: %s\n", strerror(errno));
                return 1;
            }
            CurrentChildren--;
            for (int i = 0; i < 20; i++)
            {
                if (ProcessTable[i].pid == ChildExited)
                {
                    ProcessTable[i].occupied = 0;
                    ProcessTable[i].pid = 0;
                    ProcessTable[i].nanoSeconds = 0;
                    break;
                }
            }
        }
        // for handling the first iteration of the loop
        else if (errno == ECHILD && CurrentChildren == 0){
            errno = 0;
        }
        //handles actual errors
        else if (ChildExited < 0 && errno != ECHILD){
            printf("Error: Failed to wait for child process\nError: %s\n", strerror(errno));
            return 1;
        }

        /*
        * Documenting the fork() process return values for future reference
        * For more info see: https://man7.org/linux/man-pages/man2/fork.2.html
        * Forks the process and stores the return value in IsParent
        * If the return value is 0, the process is a child
        * If the return value is -1, the process failed to fork
        * If the return value is greater than 0, the process is the parent
        */
        IsParent = fork();

        // launches the ./user program if the process is a child
        if(IsParent == 0){
            char *args[] = {"./user", MaxRuntime, NULL};
            execvp(args[0], args);
            printf("Error: Failed to launch child process\n");
            exit(EXIT_FAILURE);
        }
        else if (IsParent < 0){
            printf("Error: Failed to launch child process\n");
            exit(EXIT_FAILURE);
        
        }
        
        // keep track of the number of children
        CurrentChildren++;
        AllChildren++;

        //add the child to the process table
        for(int i = 0; i < 20; i++){
            if(ProcessTable[i].occupied == 0){
                ProcessTable[i].occupied = 1;
                ProcessTable[i].pid = IsParent;
                ProcessTable[i].nanoSeconds = value;
                break;
            }
        }
        
        
    }
    // Waits for all child processes to finish
    while(CurrentChildren > 0){
        ChildExited = waitpid(-1, &status, 0);
        if(ChildExited < 0){
            printf("Error: Failed to wait for child process\nError: %s\n", strerror(errno));
            return 1;
        }

        CurrentChildren--;
    }

    shm_unlink(NanoSeconds);

    return 0;
}


