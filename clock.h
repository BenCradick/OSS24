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
        void unlink();
        void unmap();
        void update();
        


    private:
        int NanoSecondSharedMemoryFD;
        void* NanoSecondSharedMemoryPointer;
        const char* NanoSeconds = "NanoSeconds";

        unsigned long long time;
};
#endif