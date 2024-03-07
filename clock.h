/*
    Ben Cradick
    CS4760
    Project 3
    Due: 03-07-2024 
*/
#ifndef CLOCK_H
#define CLOCK_H

class Clock
{
    public:
        Clock();
        ~Clock();
        void init();
        void incrementClock();
        unsigned long long  getTime();
        int getSeconds();
        unsigned long long getNanoSeconds();
        void setIncrement(int CurrentChildren);
        void unlink();
        void unmap();
        void update();
        


    private:
        int NanoSecondSharedMemoryFD;
        void* NanoSecondSharedMemoryPointer;

        unsigned long long time;

        unsigned long increment;
};
#endif