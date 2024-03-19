/*
    Ben Cradick
    CS4760
    Project 3
    Due: 03-07-2024 
*/
#ifndef CLOCK_H
#define CLOCK_H

#include "constants.h"

class Clock
{
    public:
        Clock();
        ~Clock();
        void init();
        void incrementClock();
        void incrementClock(ull increment);
        void scheduleIncrement();
        ull  getTime();
        int getSeconds();
        ull getNanoSeconds();

        void unlink();
        void unmap();
        void update();
        


    private:
        int NanoSecondSharedMemoryFD;
        void* NanoSecondSharedMemoryPointer;

        ull time;

};
#endif