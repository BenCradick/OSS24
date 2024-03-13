/*
    Ben Cradick
    CS4760
    Project 3
    Due: 03-07-2024 
*/



#ifndef PCB_H
#define PCB_H

#include <unistd.h>
#include "constants.h"
struct ProcessControlBlock
{
    int occupied;     
    pid_t pid;        
    long nanoSeconds;
    int blocked;
    long blockedTime;    
};

class PCB
{
    public:
          
        PCB();
        ~PCB();


        void PrintPCB();

        //  returns 0 if successful, -1 if PCB is full
        int addProcess(pid_t pid, long nanoSeconds);
        //  returns 0 if successful, -1 if PCB does not contain the process
        int removeProcess(pid_t pid);
        int getNumProcesses();

        void setBlocked(pid_t pid, long blockedTime);

        void nextProcess();


        ProcessControlBlock getPCB(int i);
        ProcessControlBlock getCurrentPCB();
        int getCurrentProcess();
        int getProccessIndex(pid_t pid);
        
    private:
        ProcessControlBlock pcb[20]; 
        int count;
        int currentProcess;



};



#endif