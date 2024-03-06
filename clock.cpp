#include "clock.h"
#include "constants.h"
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

Clock::Clock(){
    NanoSecondSharedMemoryFD = shm_open(NanoSeconds, O_CREAT | O_RDWR, 0666);
    ftruncate(NanoSecondSharedMemoryFD, sizeof(unsigned long long));
    NanoSecondSharedMemoryPointer = mmap(0, sizeof(unsigned long long), PROT_READ | PROT_WRITE, MAP_SHARED, NanoSecondSharedMemoryFD, 0);
    time = 0;
}
Clock::~Clock(){
    
}
void Clock::init(){
    memcpy(NanoSecondSharedMemoryPointer, &time, sizeof(unsigned long long));
}

void Clock::incrementClock(){
    time += increment;
    memcpy(NanoSecondSharedMemoryPointer, &time, sizeof(unsigned long long));
}
unsigned long long Clock::getTime(){
    return time;
}

int Clock::getSeconds(){
    return time / BILLION;
}
unsigned long long Clock::getNanoSeconds(){
    return time % BILLION;
}
void Clock::unlink(){
    munmap(NanoSecondSharedMemoryPointer, sizeof(unsigned long long));
    close(NanoSecondSharedMemoryFD);
    shm_unlink(NanoSeconds);
}
void Clock::unmap(){
    close(NanoSecondSharedMemoryFD);
    munmap(NanoSecondSharedMemoryPointer, sizeof(unsigned long long));
}

void Clock::update(){
    memcpy(&time, NanoSecondSharedMemoryPointer, sizeof(unsigned long long));
}
void Clock::setIncrement(int CurrentChildren){
    if(CurrentChildren == 0)
    {
        increment = 250000000;
    }
    else
    {
        increment = 250000000 / CurrentChildren;
    }
}