#include <queue>

#include "PCB.h"

static std::queue<ProcessControlBlock> q0;
static std::queue<ProcessControlBlock> q1;
static std::queue<ProcessControlBlock> q2;
static std::queue<ProcessControlBlock> blocked;