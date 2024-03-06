/*
    Ben Cradick
    CS4760
    Project 2
    Due: 02/22/2024    
*/

#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>


#include "clock.h"
#include "constants.h"
#include "message.h"



//FD is short fore file descriptor

Clock clock;
Message messageQueue;

void die(){
    clock.unmap();
    printf("Worker %d has been killed\n", getpid());
    exit(1);

}

int main(int argc,  char* argv[]){
    int seconds = atoi(argv[1]);
    int nano = atoi(argv[2]);

    signal(SIGALRM, (void (*)(int))die);
    signal(SIGINT, (void (*)(int))die);


    bool terminate = false;

    pid_t me = getpid();
    pid_t parent = getppid();


    unsigned long long  time = 0;
    unsigned long long endTime = 0;

    int endSeconds = 0;
    int endNano = 0;

    int currentSeconds = 0;
    int currentNano = 0;

    int iterations = 0;

    char message = '0';

    clock = Clock();
    messageQueue = Message();

    do {

        messageQueue.getMessage(me);

        clock.update();

        
        if(endTime == 0){
            endTime = clock.getTime() + (seconds * BILLION) + nano;
            endSeconds = clock.getSeconds() + seconds;
            endNano = clock.getNanoSeconds() + nano;

            printf("WORKER PID:%d PPID:%d SysClockSec: %d SysclockNano: %d TermTimeS: %d TermTimeNano: %d --Just Starting--\n",
            me, parent, currentSeconds, currentNano, endSeconds, endNano);

        }
        
        currentSeconds = clock.getSeconds();
        currentNano = clock.getNanoSeconds();
        
        
        
        time = clock.getTime();

        if(time >= endTime){
            terminate = true;
            message = '1';
        }

        messageQueue.sendMessage(me, &message);
        iterations++;
        printf("WORKER PID:%d PPID:%d SysClockSec: %d SysclockNano: %d TermTimeS: %d TermTimeNano: %d -- %d iterations have passed since starting\n",
                me, parent, currentSeconds, currentNano, endSeconds, endNano, iterations);
        
    
    }while(!terminate);


    clock.update();
    currentSeconds = clock.getSeconds();
    currentNano = clock.getNanoSeconds();

    printf("WORKER PID:%d PPID:%d SysClockSec: %d SysclockNano: %d TermTimeS: %d TermTimeNano: %d --Terminating after sending message back to oos after %d iterations\n",
     me, parent, currentSeconds, currentNano, endSeconds, endNano, iterations);
    
    clock.unmap();
    return 0;
}
