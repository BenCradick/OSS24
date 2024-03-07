/*
    Ben Cradick
    CS4760
    Project 3
    Due: 03-07-2024 
*/


#include <unistd.h>
#ifndef PCB_H
#define PCB_H

struct ProcessControlBlock
{
    int occupied;     
    pid_t pid;        
    unsigned long long nanoSeconds;    
};

class PCB
{
    public:
          
        PCB();
        ~PCB();


        void PrintPCB();

        //  returns 0 if successful, -1 if PCB is full
        int addProcess(pid_t pid, unsigned long long nanoSeconds);
        //  returns 0 if successful, -1 if PCB does not contain the process
        int removeProcess(pid_t pid);
        int getNumProcesses();

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