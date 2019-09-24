#include <iostream>
#include "NAsh.h"
#include <string.h>
#include <unordered_map>

enum command {QUIT, EXIT, INVALID_COMMAND};
const std::unordered_map<std::string, command> strCMD{ {"quit", QUIT}, {"exit", EXIT}};

command strToCMD(std::string s) {
    if(strCMD.find(s) == strCMD.end()) return INVALID_COMMAND;
    return strCMD.at(s);
}

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


    bool done = false; 
    while (!done) {
        // Display bash prompt
        std::cout << "NAsh> ";

        std::string strCMD;
        std::getline(std::cin, strCMD, '\n');
        char* token = std::strtok(&strCMD[0], " ");
        if(token != NULL)
        switch (strToCMD(token)) {
            // insert all other enums


            default:
                std::cout << "INVALID COMMAND" << std::endl;
            case QUIT:
            case EXIT:
                done = true;
            break;
        } 
    }
    
    return 0;
}