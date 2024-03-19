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

typedef enum {
        q0 = 0,
        q1 = 1,
        q2 = 2
    } queueType;


struct ProcessControlBlock
{
        

    int occupied;     
    pid_t pid;        
    ull nanoSeconds;
    bool blocked;
    ull blockedTime;  

    queueType queue;  

};

class PCB
{
    public:
          
        PCB();
        ~PCB();


        void PrintPCB();

        //  returns 0 if successful, -1 if PCB is full
        int addProcess(pid_t pid, ull nanoSeconds);
        //  returns 0 if successful, -1 if PCB does not contain the process
        int removeProcess(pid_t pid);
        int getNumProcesses();

        void updateProcess(ProcessControlBlock pcb);

        void setBlocked(pid_t pid, ull blockedTime);

        void nextProcess();


        ProcessControlBlock getPCB(int i);
        ProcessControlBlock getCurrentPCB();
        int getCurrentProcess();
        ProcessControlBlock getProccessByPid(pid_t pid);
        
    private:
        ProcessControlBlock pcb[20]; 
        int count;
        int currentProcess;



};



#endif