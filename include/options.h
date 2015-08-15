#ifndef OPTIONS_H
#define OPTIONS_H

#include <iostream>

struct Options {
    std::string clausifier;
    std::string solver;
    std::string structure;
    int verbosity;
    
    Options();
    Options(int argc, char **argv);
    void printHelp();
};

#endif