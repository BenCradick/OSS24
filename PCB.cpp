#include "PCB.h"
#include "constants.h"
#include <stdio.h>

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
}

PCB::~PCB()
{
}

void PCB::PrintPCB()
{
    printf("%-17s %-17s %-17s %-17s %-17s\n", "Entry:", "Occupied:", "PID:", "StartSeconds:", "StartNanoSeconds:");
    for(int i = 0; i < 20; i++)
    {
        printf("%-17d %-17d %-17d %-17llu %-17llu\n", i, pcb[i].occupied, pcb[i].pid, pcb[i].nanoSeconds / BILLION, pcb[i].nanoSeconds % BILLION);
    }
}
void PCB::nextProcess()
{
    while(pcb[currentProcess].occupied == 0){
        if(currentProcess < 19)
        {
            currentProcess++;
        }
        else
        {
            currentProcess = 0;
        }
    }
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