#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>

void format(int* seconds, int* nano);

int main(int argc,  char* argv[]){
    int seconds = atoi(argv[1]);
    int nano = atoi(argv[2]);

    const int BILLION = 1000000000;

    unsigned long lastTime = 0;

    pid_t me = getpid();
    pid_t parent = getppid();


    unsigned long  time = 0;
    unsigned long endTime = 0;

    int endSeconds = 0;
    int endNano = 0;

    int currentSeconds = 0;
    int currentNano = 0;

    int startSeconds = 0;
    


    const int SIZE = 8;
    
    const char* NanoSeconds = "NanoSeconds";


    //FD is short fore file descriptor

    int NanoSecondSharedMemoryFD;
    void* NanoSecondSharedMemoryPointer;

    


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

    printf("WORKER PID:%d PPID:%d SysClockS: %d SysclockNano: %d TermTimeS: %d TermTimeNano: %d --Just Starting--\n",
     me, parent, currentSeconds, currentNano, endSeconds, endNano);

    endTime = (seconds * BILLION) + nano + time;
    lastTime = time;

    while(time < endTime){
        // void *memcpy(void *dest, const void * src, size_t n)
        memcpy(&time, NanoSecondSharedMemoryPointer, SIZE);        

        if(time >= lastTime + 1000000){
            lastTime = time;

            currentSeconds = (time / BILLION);
            currentNano = (time % BILLION);
            format(&currentSeconds, &currentNano);

            endSeconds = currentSeconds + seconds;
            endNano = currentNano + nano;
            format(&endSeconds, &endNano);
            
            printf("WORKER PID:%d PPID:%d SysClockS: %d SysclockNano: %d TermTimeS: %d TermTimeNano: %d -- %d seconds have passed since starting\n",
                   me, parent, currentSeconds, currentNano, endSeconds, endNano, currentSeconds - startSeconds);
        }
    
    }
    printf("WORKER PID:%d PPID:%d SysClockS: %d SysclockNano: %d TermTimeS: %d TermTimeNano: %d --Just Exiting--\n",
     me, parent, currentSeconds, currentNano, endSeconds, endNano);
}
void format(int* seconds, int* nano){
    if(*nano >= 1000000000){
        *seconds += 1;
        *nano -= 1000000000;
    }
}