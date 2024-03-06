/*
    Ben Cradick
    CS4760
    Project 2
    Due: 02/22/2024    
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
#include <fstream>
#include <iostream>

#include "constants.h"
#include "PCB.h"
#include "clock.h"
#include "message.h"



#pragma region GlobalVariables


//global variables needed for killing shared memory via signals.
//FD is short fore file descriptor

Clock clock;
PCB pcb;
Message messageQueue;

// struct ProcessControlBlock
// {
//     int occupied;     
//     pid_t pid;        
//     unsigned long long nanoSeconds;    
// };
// struct ProcessControlBlock ProcessTable[20];
#pragma endregion

#pragma region FunctionPrototypes
//function prototypes


void killChildren();

char* itoa(int num, char* str, int base);
void reverse(char str[], int length);

#pragma endregion

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

    std::string logFileName = "log.txt";

    unsigned long long value = 0;
    unsigned long long increment = 135;

    unsigned long long nextTime = 0;
    unsigned long long nextPrint = BILLION / 2;

    std::ofstream logFile;
      

    char secChar[33];
    char nanoChar[33];

    pcb = PCB();

    clock = Clock();
    clock.init();

    messageQueue = Message(messageTypes::PARENT);

    #pragma endregion




    


    // set up the alarm and signal handling
    signal(SIGALRM, (void (*)(int))killChildren);
    signal(SIGINT, (void (*)(int))killChildren);
    alarm(60);
    #pragma region ArgumentParsing
    while((Opt = getopt(argc, argv, "n:s:t:i:f:h")) != -1){
        // opt arguments -n, -s, -t -i, -h
        switch(Opt){
            case 'h':
                printf("./oss [-n proc] [-s simul] [-t timelimitForChildren] [-i intervalInMSToLaunchChildren] [-h]\n");
                printf("proc: number of processes to run\n");
                printf("simul: number of processes to run at a time\n");
                printf("iter: Maximum runtime\n");
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
            case 'f':
                logFileName = optarg;
                logFile.open(optarg);
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
            printf("iter: Maximum runtime\n");
            printf("intervalInMsToLaunchChildren: iterval between process launches in ms\n");
            printf("logFile: name of the log file\n");

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


    
    #pragma region MainLoop
    
    do{
        

        /*
        * Documenting the fork() process return values for future reference
        * For more info see: https://man7.org/linux/man-pages/man2/fork.2.html
        * Forks the process and stores the return value in IsParent
        * If the return value is 0, the process is a child
        * If the return value is -1, the process failed to fork
        * If the return value is greater than 0, the process is the parent
        */

       

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

            //add the child to the process table

            pcb.addProcess(IsParent, value);

        }
        else if(IsParent == -1){
                printf("Error: Failed to wait for child process\nError: %s\n", strerror(errno));
                return 1;
            }
        
        IsParent = -2;
       
        if(nextTime <= value){
            nextTime = value + launchInterval * MILLION;
        }
    
        //check for finished children without hanging
        
        ChildExited = waitpid(-1, &status, WNOHANG);
        if(ChildExited > 0){
            CurrentChildren--;
            
            pcb.removeProcess(ChildExited);
        }


        //handles actual errors
        else if (ChildExited < 0 && errno != ECHILD){
            printf("Error: Failed to wait for child process\nError: %s\n", strerror(errno));
            return 1;
        }

       if(value >= nextPrint){
            
            
            pcb.PrintPCB();
            
            while(nextPrint <= value){
                nextPrint += BILLION / 2;
            }
        }
        //nextTime = value + launchInterval * MILLION;
        clock.incrementClock();
        value = clock.getTime();

        

        
        
    // AllChildren is number of chilren run, TotalChildren is the number specified.
    }while(CurrentChildren > 0);
    #pragma endregion

    pcb.PrintPCB();



    return 0;
}

#pragma region Functions
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
    if (isNegative){
        str[i++] = '-';
    }
 
    str[i] = '\0'; // Append string terminator
 
    // Reverse the string
    reverse(str, i);
 
    return str;
}

void killChildren(){
    for(int i = 0; i < 20; i++){
        kill(pcb.getPCB(i).pid, SIGKILL);
    }
    
    exit(0);
}
#pragma endregion
