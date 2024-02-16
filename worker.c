#include <stdio.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>



int main(int argc,  char* argv[]){
    int seconds = atoi(argv[1]);
    int nano = atoi(argv[2]);

    const int BILLION = 1000000000;

    unsigned long lastTime = 0;

    pid_t me = getpid();
    pid_t parent = getppid();


    unsigned long  time = 0;
    unsigned long endTime = 0;


    const int SIZE = 8;
    
    const char* NanoSeconds = "NanoSeconds";


    //FD is short fore file descriptor

    int NanoSecondSharedMemoryFD;
    void* NanoSecondSharedMemoryPointer;

    


    NanoSecondSharedMemoryFD = shm_open(NanoSeconds, O_CREAT | O_RDWR, 0666);


    ftruncate(NanoSecondSharedMemoryFD, SIZE);

    


    NanoSecondSharedMemoryPointer = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, NanoSecondSharedMemoryFD, 0);

    memcpy(NanoSecondSharedMemoryPointer, &time, SIZE);

    endTime = (seconds * BILLION) + nano + time;
    lastTime = time;

    while(time < endTime){
        memcpy(&time, NanoSecondSharedMemoryPointer, SIZE);        

        if(time >= lastTime + 1000000){
            lastTime = time;

        }
    
    }
    



}