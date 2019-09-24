#include <iostream>
#include "NAsh.h"
#include <string>

int main (int argc, char **argv, char **envp) {

    if(argc > 1 && (strncmp(argv[1], "-v", 3) == 0 || strncmp(argv[1], "--version", 10))) {
        std::cout << "v0.0.1: Nathan Nichols and Andre Kurait: NAsh" << std::endl;
        return 0;
    }

    while (true) {
        // Display bash prompt
        std::cout << "NAsh> ";
        exit;
    }
    
    return 0;
}