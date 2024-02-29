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

void format(int* seconds, int* nano);

//FD is short fore file descriptor

Clock clock;

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



    unsigned long long lastTime = 0;

    pid_t me = getpid();
    pid_t parent = getppid();


    unsigned long long  time = 0;
    unsigned long long endTime = 0;

    int endSeconds = 0;
    int endNano = 0;

    int currentSeconds = 0;
    int currentNano = 0;

    int startSeconds = 0;

    clock = Clock();
    


    
    
    


    clock.update();

    currentSeconds = clock.getSeconds();
    currentNano = clock.getNanoSeconds();

    startSeconds = currentSeconds;

    endSeconds = currentSeconds + seconds;
    endNano = currentNano + nano;

    printf("WORKER PID:%d PPID:%d SysClockSec: %d SysclockNano: %d TermTimeS: %d TermTimeNano: %d --Just Starting--\n",
     me, parent, currentSeconds, currentNano, endSeconds, endNano);

    endTime = (seconds * BILLION) + nano + clock.getTime();
    lastTime = time;


    while(clock.getTime() < endTime){

        // printf("endTime: %lu, time: %lu diff: %lu\n", endTime, time, endTime - time);
        // void *memcpy(void *dest, const void * src, size_t n)
        clock.update();

               

        if(clock.getTime() >= lastTime + BILLION){
            lastTime = clock.getTime();

            currentSeconds = clock.getSeconds();
            currentNano = clock.getNanoSeconds();


            
            
            printf("WORKER PID:%d PPID:%d SysClockSec: %d SysclockNano: %d TermTimeS: %d TermTimeNano: %d -- %d seconds have passed since starting\n",
                   me, parent, currentSeconds, currentNano, endSeconds, endNano, currentSeconds - startSeconds);
        }
    
    }
    clock.update();
    currentSeconds = clock.getSeconds();
    currentNano = clock.getNanoSeconds();

    printf("WORKER PID:%d PPID:%d SysClockSec: %d SysclockNano: %d TermTimeS: %d TermTimeNano: %d --Terminating\n",
     me, parent, currentSeconds, currentNano, endSeconds, endNano);
    
    clock.unmap();
    return 0;
}
