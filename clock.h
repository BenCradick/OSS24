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
        ull  getTime();
        int getSeconds();
        ull getNanoSeconds();
        void setIncrement(int CurrentChildren);
        void unlink();
        void unmap();
        void update();
        


    private:
        int NanoSecondSharedMemoryFD;
        void* NanoSecondSharedMemoryPointer;

        ull time;

        unsigned long increment;
};
#endif