#include <iostream>
#include "NAsh.h"
#include <string.h>

int main (int argc, char **argv, char **envp) {

    if(argc > 1 && (strncmp(argv[1], "-v", 3) == 0 || strncmp(argv[1], "--version", 10) == 0)) {
        std::cout << "v0.0.1: Nathan Nichols and Andre Kurait: NAsh" << std::endl;
        return 0;
    }
    else if (argc > 1 && strncmp(argv[1], "--help", 7) == 0) {
        std::cout << "insert help menu here" << std::endl;
        return 0;
    }
    
    while (true) {
        // Display bash prompt
        std::cout << "NAsh> ";
        break;
    }
    
    return 0;
}