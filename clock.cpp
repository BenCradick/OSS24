/*
    Ben Cradick
    CS4760
    Project 3
    Due: 03-07-2024 
*/
#include "clock.h"
#include "constants.h"
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

Clock::Clock(){
    NanoSecondSharedMemoryFD = shm_open(NanoSeconds, O_CREAT | O_RDWR, 0666);
    ftruncate(NanoSecondSharedMemoryFD, sizeof(ull));
    NanoSecondSharedMemoryPointer = mmap(0, sizeof(ull), PROT_READ | PROT_WRITE, MAP_SHARED, NanoSecondSharedMemoryFD, 0);
    time = 0;
}
Clock::~Clock(){
    
}
void Clock::init(){
    memcpy(NanoSecondSharedMemoryPointer, &time, sizeof(ull));
}

void Clock::incrementClock(){
    time += increment;
    memcpy(NanoSecondSharedMemoryPointer, &time, sizeof(ull));
}
ull Clock::getTime(){
    return time;
}

int Clock::getSeconds(){
    return time / BILLION;
}
ull Clock::getNanoSeconds(){
    return time % BILLION;
}
void Clock::unlink(){
    munmap(NanoSecondSharedMemoryPointer, sizeof(ull));
    close(NanoSecondSharedMemoryFD);
    shm_unlink(NanoSeconds);
}
void Clock::unmap(){
    close(NanoSecondSharedMemoryFD);
    munmap(NanoSecondSharedMemoryPointer, sizeof(ull));
}

void Clock::update(){
    memcpy(&time, NanoSecondSharedMemoryPointer, sizeof(ull));
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