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
#include <stdbool.h>



#define BILLION 1000000000
#define MILLION 1000000

struct ProcessControlBlock
{
    int occupied;     
    pid_t pid;        
    unsigned long long nanoSeconds;    
};
struct ProcessControlBlock ProcessTable[20];

void printProcessControlBlock(struct ProcessControlBlock pcb[], int size);

void killChildren(){
    for(int i = 0; i < 20; i++){
        if(ProcessTable[i].occupied == 1){
            kill(ProcessTable[i].pid, SIGKILL);
        }
    }
    exit(0);
}

char* itoa(int num, char* str, int base);
void reverse(char str[], int length);

int main(int argc,  char* argv[]){
    #pragma region Variables
    int Opt;
    int TotalChildren = -1;
    int MaxProcess = -1;
    int MaxRuntime = -1;
    int launchInterval = -1;
    pid_t IsParent = -2;
    pid_t ChildExited = -1;
    int CurrentChildren =  0;
    int AllChildren = 0;
    int status;

    int nanoMax = 0;
    int secMax = 0;

    unsigned long long value = 0;
    unsigned long long increment = 100000;

    unsigned long long nextTime = 0;
    unsigned long long nextPrint = BILLION / 2;
    

    const int SIZE = sizeof(unsigned long long);
    
    const char* NanoSeconds = "NanoSeconds";

    char secChar[33];
    char nanoChar[33];

    //FD is short fore file descriptor

    int NanoSecondSharedMemoryFD;
    void* NanoSecondSharedMemoryPointer;

    #pragma endregion

    
    #pragma region SharedMemory

    NanoSecondSharedMemoryFD = shm_open(NanoSeconds, O_CREAT | O_RDWR, 0666);


    ftruncate(NanoSecondSharedMemoryFD, SIZE);

    


    NanoSecondSharedMemoryPointer = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, NanoSecondSharedMemoryFD, 0);

    memcpy(NanoSecondSharedMemoryPointer, &value, SIZE);
    #pragma endregion




    //initialize the process table
    for(int i = 0; i < 20; i++){
                ProcessTable[i].occupied = 0;
                ProcessTable[i].pid = 0;
                ProcessTable[i].nanoSeconds = 0;                   
        }


    // set up the alarm
    signal(SIGALRM, (void (*)(int))killChildren);
    alarm(60);
    #pragma region ArgumentParsing
    while((Opt = getopt(argc, argv, "n:s:t:i:h")) != -1){
        // opt arguments -n, -s, -t -i, -h
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
                break;
            case 's':
                MaxProcess = atoi(optarg);
                if(MaxProcess >= 20){
                    MaxProcess = 20;
                }
                break;
            case 't':
                MaxRuntime = atoi(optarg);
                secMax = rand() % MaxRuntime + 1;
                nanoMax = rand() % BILLION;              
                break;
            case 'i':
                launchInterval = atoi(optarg);
                break;
            default:
                printf("Option ?\n");
                break;
        }

        
    }
    if(TotalChildren == -1 || MaxProcess == -1 || MaxRuntime == -1){
            printf("Missing argument, assigning obnoxious default values, see the help menu.\n");
            printf("./oss [-n proc] [-s simul] [-t iter] [-h]\n");
            printf("proc: number of processes to run\n");
            printf("simul: number of processes to run at a time\n");
            printf("iter: number of iterations for user to run\n");
            printf("intervalInMsToLaunchChildren: iterval between process launches in ms\n");

            if(TotalChildren == -1){
                TotalChildren = 19;
            }
            if(MaxProcess == -1){
                MaxProcess = 10;
            }
            if(MaxRuntime == -1){
                MaxRuntime = 7;
            }
            if(launchInterval == -1){
                launchInterval = 100;
            }
            
        }
    #pragma endregion


    

    // AllChildren is number of chilren run, TotalChildren is the number specified.
    while(AllChildren < TotalChildren){
        
        

        if(value >= nextPrint){
            
            
            printProcessControlBlock(ProcessTable, 20);
            
            while(nextPrint <= value){
                nextPrint += BILLION / 2;
            }
        }

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

       

        // printf statement for the values in the following if statement
        //printf("CurrentChildren: %d MaxProcess: %d AllChildren: %d TotalChildren: %d NextTime: %llu Value: %llu \n",  CurrentChildren, MaxProcess, AllChildren, TotalChildren, nextTime, value);
        if(CurrentChildren < MaxProcess && AllChildren < TotalChildren && nextTime <= value){
            IsParent = fork();
        }
        // launches the ./user program if the process is a child
        if(IsParent == 0){
            char *args[] = {"./worker", itoa(secMax, secChar, 10), itoa(nanoMax, nanoChar, 10), NULL};
            execvp(args[0], args);
            printf("Error: Failed to launch child process\n");
            exit(EXIT_FAILURE);
        }
        else if (IsParent > 0){
            // keep track of the number of children
            CurrentChildren++;
            AllChildren++;

            printf("sec: %llu, nano: %llu\n", value/BILLION, value %BILLION);

            //printf("AllChildren: %d CurrentChildren: %d\n", CurrentChildren, AllChildren);

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
        else if(IsParent == -1){
                printf("Error: Failed to wait for child process\nError: %s\n", strerror(errno));
                return 1;
            }
        
        IsParent = -2;
       
       if(nextTime <= value){
           nextTime = value + launchInterval * MILLION;
       }
        //nextTime = value + launchInterval * MILLION;
        value += increment;
        memcpy(NanoSecondSharedMemoryPointer, &value, sizeof(value));

        
        
        
    }
    // Waits for all child processes to finish
    while(CurrentChildren > 0){
        
        
        ChildExited = waitpid(-1, &status, WNOHANG);
        if(ChildExited < 0){
            printf("Error: Failed to wait for child process\nError: %s\n", strerror(errno));
            return 1;
        }
        if(ChildExited > 0){
            CurrentChildren--;
            
        }
        value += increment;
        memcpy(NanoSecondSharedMemoryPointer, &value, sizeof(value));
        
        
    }

    shm_unlink(NanoSeconds);

    return 0;
}


void reverse(char str[], int length)
{
    int start = 0;
    int end = length - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        end--;
        start++;
    }
}


// shamelessly stolen from https://www.geeksforgeeks.org/implement-itoa/

char* itoa(int num, char* str, int base)
{
    int i = 0;
    bool isNegative = false;
 
    /* Handle 0 explicitly, otherwise empty string is
     * printed for 0 */
    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }
 
    // In standard itoa(), negative numbers are handled
    // only with base 10. Otherwise numbers are
    // considered unsigned.
    if (num < 0 && base == 10) {
        isNegative = true;
        num = -num;
    }
 
    // Process individual digits
    while (num != 0) {
        int rem = num % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        num = num / base;
    }
 
    // If number is negative, append '-'
    if (isNegative)
        str[i++] = '-';
 
    str[i] = '\0'; // Append string terminator
 
    // Reverse the string
    reverse(str, i);
 
    return str;
}

void printProcessControlBlock(struct ProcessControlBlock table[], int size){
    //attempting to keep the program from seperating lines when printing but its still getting interrupted
    printf("%-17s %-17s %-17s %-17s %-17s\n", "Entry:", "Occupied:", "PID:", "StartSeconds:", "StartNanoSeconds:");
    for (int i = 0; i < size; i++) {
        printf("%d\t%-17d\t%-17d\t%-17llu\t%-17llu\n", i, table[i].occupied, table[i].pid, table[i].nanoSeconds / BILLION, table[i].nanoSeconds % BILLION);
    }
}
