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

    std::cout <<  "______________________" << std::endl;
    std::cout << "< Welcome to NAsh u ho >" << std::endl;
    std::cout << " ----------------------" << std::endl;
    std::cout << "\t\\   ^__^" << std::endl;
    std::cout << "\t \\  (oo)\\_______" << std::endl;
    std::cout << "\t    (__)\\       )\\/" << std::endl;
    std::cout << "\t\t||----w |" << std::endl;
    std::cout << "\t\t||     ||" << std::endl;

    std::string cmd;
    while (true) {
        // Display bash prompt
        std::cout << "NAsh> ";
        std::getline(std::cin, cmd, '\n');
        break;
    }
    
    return 0;
}