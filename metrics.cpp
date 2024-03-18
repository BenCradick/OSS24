#include "metrics.h"
#include "queues.h"

Metrics::Metrics() {
    blockTime = 0;
    cpuTime = 0;
    idleTime = 0;
}

Metrics::~Metrics() {
    // Nothing to do here
}

void Metrics::incrementBlocked(ull quantum) {
    blockTime += (quantum * blocked.size());
}

void Metrics::incrementCPU(ull quantum) {
    cpuTime += quantum;
}

void Metrics::incrementIdle(ull quantum){
    idleTime += quantum;
}