
#ifndef METRICS_H
#define METRICS_H

#include <iostream>
#include <queue>

#include "constants.h"
#include "logger.h"
#include "PCB.h"
#include "queues.h"

class Metrics {
    public: 
        Metrics();
        ~Metrics();
        void incrementBlocked(ull quantum);
        void incrementCPU(ull quantum);
        void incrementIdle(); 
    
    private:
        ull blockTime;
        ull cpuTime;
        ull idleTime;
};

#endif