#include "PCB.h"
#include "constants.h"
#include "logger.h"
#include <stdio.h>
#include <iostream>
#include <iomanip>

PCB::PCB()
{
    count = 0;
    currentProcess = 0;
    for(int i = 0; i < 20; i++)
    {
        pcb[i].occupied = 0;
        pcb[i].pid = 0;
        pcb[i].nanoSeconds = 0;
    }
    first = true;
}

PCB::~PCB()
{
}

void PCB::PrintPCB()
{
    logger() << std::left << std::setw(17) << "Entry:" 
          << std::setw(17) << "Occupied:" 
          << std::setw(17) << "PID:" 
          << std::setw(17) << "StartSeconds:" 
          << std::setw(17) << "StartNanoSeconds:" << std::endl;

for(int i = 0; i < 20; i++)
{
    logger() << std::left << std::setw(17) << i 
              << std::setw(17) << pcb[i].occupied 
              << std::setw(17) << pcb[i].pid 
              << std::setw(17) << pcb[i].nanoSeconds / BILLION 
              << std::setw(17) << pcb[i].nanoSeconds % BILLION << std::endl;
}
}
void PCB::nextProcess()
{
    //this way we start iterating at current process and loop around to the beginning
    int i = currentProcess;
    do
    {
        i++;
    }while(pcb[i % 20].occupied == 0);
    currentProcess = i % 20;
}



int PCB::addProcess(pid_t pid, unsigned long long nanoSeconds)
{
    if(count < 20){
        for(int i = 0; i < 20; i++)
        {
            if(pcb[i].occupied == 0)
            {
                pcb[i].occupied = 1;
                pcb[i].pid = pid;
                pcb[i].nanoSeconds = nanoSeconds;
                count++;
                return 0;
            }
        }
        return -1;
    }
    return -1;
}

int PCB::removeProcess(pid_t pid)
{
    for(int i = 0; i < 20; i++)
    {
        if(pcb[i].pid == pid)
        {
            pcb[i].occupied = 0;
            pcb[i].pid = 0;
            pcb[i].nanoSeconds = 0;
            count--;
            return 0;
        }
    }
    return -1;
}

int PCB::getNumProcesses()
{
    return count;
}

ProcessControlBlock PCB::getPCB(int i)
{
    return pcb[i];
}
ProcessControlBlock PCB::getCurrentPCB()
{
    return pcb[currentProcess];
}
int PCB::getCurrentProcess()
{
    return currentProcess;
}
int PCB::getProccessIndex(pid_t pid)
{
    for(int i = 0; i < 20; i++)
    {
        if(pcb[i].pid == pid)
        {
            return i;
        }
    }
    return -1;
}