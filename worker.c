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

void format(int* seconds, int* nano);

//FD is short fore file descriptor

int NanoSecondSharedMemoryFD;
void* NanoSecondSharedMemoryPointer;
const int SIZE = sizeof(unsigned long long);

void die(){
    printf("Worker %d has been killed\n", getpid());
    close(NanoSecondSharedMemoryFD);
    munmap(NanoSecondSharedMemoryPointer, SIZE);
    exit(1);

}

int main(int argc,  char* argv[]){
    int seconds = atoi(argv[1]);
    int nano = atoi(argv[2]);

    signal(SIGALRM, (void (*)(int))die);
    signal(SIGINT, (void (*)(int))die);

    const unsigned long long BILLION = 1000000000;

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
    


    
    
    const char* NanoSeconds = "NanoSeconds";


    

    


    NanoSecondSharedMemoryFD = shm_open(NanoSeconds, O_CREAT | O_RDWR, 0666);


    ftruncate(NanoSecondSharedMemoryFD, SIZE);

    


    NanoSecondSharedMemoryPointer = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, NanoSecondSharedMemoryFD, 0);

    memcpy(&time, NanoSecondSharedMemoryPointer, SIZE);

    currentSeconds = (time / BILLION);
    currentNano = (time % BILLION);

    format(&currentSeconds, &currentNano);
    startSeconds = currentSeconds;

    endSeconds = currentSeconds + seconds;
    endNano = currentNano + nano;
    format(&endSeconds, &endNano);

    printf("WORKER PID:%d PPID:%d SysClockSec: %d SysclockNano: %d TermTimeS: %d TermTimeNano: %d --Just Starting--\n",
     me, parent, currentSeconds, currentNano, endSeconds, endNano);

    endTime = (seconds * BILLION) + nano + time;
    lastTime = time;


    while(time < endTime){

        // printf("endTime: %lu, time: %lu diff: %lu\n", endTime, time, endTime - time);
        // void *memcpy(void *dest, const void * src, size_t n)
        memcpy(&time, NanoSecondSharedMemoryPointer, SIZE);

               

        if(time >= lastTime + BILLION){
            lastTime = time;

            currentSeconds = (time / BILLION);
            currentNano = (time % BILLION);

            
            format(&currentSeconds, &currentNano);


            
            
            printf("WORKER PID:%d PPID:%d SysClockSec: %d SysclockNano: %d TermTimeS: %d TermTimeNano: %d -- %d seconds have passed since starting\n",
                   me, parent, currentSeconds, currentNano, endSeconds, endNano, currentSeconds - startSeconds);
        }
    
    }
    printf("WORKER PID:%d PPID:%d SysClockSec: %d SysclockNano: %d TermTimeS: %d TermTimeNano: %d --Terminating\n",
     me, parent, currentSeconds, currentNano, endSeconds, endNano);
    
    close(NanoSecondSharedMemoryFD);
    munmap(NanoSecondSharedMemoryPointer, SIZE);
    return 0;
}
void format(int* seconds, int* nano){
    if(*nano >= 1000000000){
        *seconds += 1;
        *nano -= 1000000000;
    }
}