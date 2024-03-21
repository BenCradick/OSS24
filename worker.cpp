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
    ull seconds = atoi(argv[1]);
    int nano = atoi(argv[2]);

    signal(SIGALRM, (void (*)(int))die);
    signal(SIGINT, (void (*)(int))die);


    bool terminate = false;
    bool blocked = false;

    pid_t me = getpid();
    pid_t parent = getppid();

    srand(me);
    int random;


    ull  time = 0;
    ull endTime = 0;
    ull timeUsed = 0;

    int endSeconds = 0;
    int endNano = 0;

    int currentSeconds = 0;
    int currentNano = 0;

    int iterations = 0;

    char message = '0';

    messageBuffer buf;

    sysClock = Clock();
    messageQueue = Message(messageTypes::CHILD);

    do {

        buf = messageQueue.getMessage(me, 0);

        sysClock.update();

         random = rand() % 100;

        
        if(endTime == 0){
            ull temp = sysClock.getTime();
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
            // write termination lifecycle
            terminate = true;
            message = '1';
        }
        else if(random < 10){
            // write blocked lifecycle
            blocked = true;
            message = '2';
        }
        else if(random < 90){
            // write normal lifecycle
            message = '0';
        }
        else{
            // write terminate lifecycle
            message = '0';
        }

        

        buf.mtype = messageTypes::PARENT;
        buf.pid = me;
        buf.terminate = terminate;
        buf.blocked = blocked;
        buf.timeUsed = timeUsed;

        messageQueue.sendMessage(buf);
        iterations++;
        printf("WORKER PID:%d PPID:%d SysClockSec: %d SysclockNano: %d TermTimeS: %d TermTimeNano: %d -- %d iterations have passed since starting\n",
                me, parent, currentSeconds, currentNano, endSeconds, endNano, iterations);
        
    
    }while(!buf.terminate);


    sysClock.update();
    currentSeconds = sysClock.getSeconds();
    currentNano = sysClock.getNanoSeconds();

    printf("WORKER PID:%d PPID:%d SysClockSec: %d SysclockNano: %d TermTimeS: %d TermTimeNano: %d --Terminating after sending message back to oos after %d iterations\n",
     me, parent, currentSeconds, currentNano, endSeconds, endNano, iterations);
    
    sysClock.unmap();
    return 0;
}
