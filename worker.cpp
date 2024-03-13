/*
    Ben Cradick
    CS4760
    Project 3
    Due: 03-07-2024 
*/
#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>


#include "clock.h"
#include "constants.h"
#include "message.h"



//FD is short fore file descriptor

Clock sysClock;
Message messageQueue;

void die(){
    sysClock.unmap();
    printf("Worker %d has been killed\n", getpid());
    exit(1);

}

int main(int argc,  char* argv[]){
    long seconds = atoi(argv[1]);
    int nano = atoi(argv[2]);

    signal(SIGALRM, (void (*)(int))die);
    signal(SIGINT, (void (*)(int))die);


    bool terminate = false;

    pid_t me = getpid();
    pid_t parent = getppid();


    long  time = 0;
    long endTime = 0;

    int endSeconds = 0;
    int endNano = 0;

    int currentSeconds = 0;
    int currentNano = 0;

    int iterations = 0;

    char message = '0';

    sysClock = Clock();
    messageQueue = Message(messageTypes::CHILD);

    do {

        messageQueue.getMessage(me, 0);

        sysClock.update();

        
        if(endTime == 0){
            long temp = sysClock.getTime();
            endTime = temp + (seconds * BILLION) + nano;

            endSeconds = sysClock.getSeconds() + seconds;
            endNano = sysClock.getNanoSeconds() + nano;

            printf("WORKER PID:%d PPID:%d SysClockSec: %d SysclockNano: %d TermTimeS: %d TermTimeNano: %d --Just Starting--\n",
            me, parent, currentSeconds, currentNano, endSeconds, endNano);

        }
        
        
        
        
        
        time = sysClock.getTime();

        currentSeconds = sysClock.getSeconds();
        currentNano = sysClock.getNanoSeconds();

        if(time >= endTime){
            terminate = true;
            message = '1';
        }

        messageQueue.sendMessage(me, &message);
        iterations++;
        printf("WORKER PID:%d PPID:%d SysClockSec: %d SysclockNano: %d TermTimeS: %d TermTimeNano: %d -- %d iterations have passed since starting\n",
                me, parent, currentSeconds, currentNano, endSeconds, endNano, iterations);
        
    
    }while(!terminate);


    sysClock.update();
    currentSeconds = sysClock.getSeconds();
    currentNano = sysClock.getNanoSeconds();

    printf("WORKER PID:%d PPID:%d SysClockSec: %d SysclockNano: %d TermTimeS: %d TermTimeNano: %d --Terminating after sending message back to oos after %d iterations\n",
     me, parent, currentSeconds, currentNano, endSeconds, endNano, iterations);
    
    sysClock.unmap();
    return 0;
}
