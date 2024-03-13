/*
    Ben Cradick
    CS4760
    Project 3
    Due: 03-07-2024 
*/
#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <fstream>

#define BILLION 1000000000
#define MILLION 1000000

#define long unsigned long long

inline const char* NanoSeconds = "NanoSeconds";

// must be opened in main to be used in logger.h
extern std::ofstream logFile;
#endif