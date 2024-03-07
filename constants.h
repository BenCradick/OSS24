#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <fstream>

#define BILLION 1000000000
#define MILLION 1000000

inline const char* NanoSeconds = "NanoSeconds";

// must be opened in main to be used in logger.h
extern std::ofstream logFile;
#endif