#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>
#include <fstream>
#include <iostream>
#include "constants.h"

// inpired/stolen from this stackoverflow answer: https://stackoverflow.com/questions/21232517/writing-to-both-terminal-and-file-c
struct LogStream 
{
    template<typename T> 
    LogStream& operator<<(const T& mValue)
    {
        std::cout << mValue;
        logFile << mValue;
        return *this;
    }

    LogStream& operator<<(std::ostream& (*func)(std::ostream&))
    {
        std::cout << func;
        logFile << func;
        return *this;
    }
};

inline LogStream& logger() { static LogStream l; return l; }
#endif