#pragma once
#include "vector"

#define DEBUG = 0
#define INFO = 1
#define WARN = 2
#define ERROR = 3

class Logger {
private:
    std::vector<Logger> loggers;

public:
    int install(std::vector<Logger> loggers);



};
