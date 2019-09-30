#include <iostream>
#include "NAsh.h"
#include <string.h>
#include <unordered_map>
#include <signal.h>

enum command {QUIT, EXIT, SET, INVALID_COMMAND};
const std::unordered_map<std::string, command> strCMD{ {"quit", QUIT}, {"exit", EXIT}, {"set", SET} };

void sigintHandler(int) 
{   
    std::cout << "\nNAsh> " << std::flush;
} 

command strToCMD(std::string s) {
    if(strCMD.find(s) == strCMD.end()) return INVALID_COMMAND;
    if(std::cin.eof()) return QUIT;
    return strCMD.at(s);
}

int main (int argc, char **argv, char **envp) {
    NAsh shell(envp);
    signal(SIGINT, sigintHandler);

    //Populates Environment variables
    for(char** i = envp; *i != 0; i++) {
        char* envVar = strtok(*i, "=");
        char* envVal = strtok(NULL, " ");
        shell.appendEnv(envVar, envVal);
    }
    if(argc > 1 && (strncmp(argv[1], "-v", 3) == 0 || strncmp(argv[1], "--version", 10) == 0)) {
        std::cout << "v0.0.1: Nathan Nichols and Andre Kurait: NAsh" << std::endl;
        return 0;
    }
    else if (argc > 1 && strncmp(argv[1], "--help", 7) == 0) {
        std::cout << "Usage: nash <arguments> <options>" << std::endl << std::endl;
        std::cout << "Options:\n\t --help\t\t display help message, exits\n\t --version -v\t show version number, exits\n";
        std::cout << std::endl;
        return 0;
    }

    std::cout <<  " _________________" << std::endl;
    std::cout << "< Welcome to NAsh >" << std::endl;
    std::cout << " -----------------" << std::endl;
    std::cout << "\t\\   ^__^" << std::endl;
    std::cout << "\t \\  (oo)\\_______" << std::endl;
    std::cout << "\t    (__)\\       )\\/" << std::endl;
    std::cout << "\t\t||----w |" << std::endl;
    std::cout << "\t\t||     ||" << std::endl;

    bool done = false; 
    std::string strCMD;
    while (!done && std::cout << "NAsh> " && std::getline(std::cin, strCMD, '\n')) {
        char* token = strtok(&strCMD[0], " ");
        if(token != NULL)

        //TODO: replace switch command with map<char*, std::function<int(...)>>
        switch (strToCMD(token)) {
            // insert all other enums
            case SET: {
                char* envVar = strtok(NULL, "=");
                char* envVal = strtok(NULL, " ");
                if(envVar != NULL && envVal != NULL) {
                    std::cout << "var: " << envVar << std::endl;
                    std::cout << "val: " << envVal << std::endl;
                } else {
                    std::cout << "Invalid \"set\" syntax. Correct syntax is \"set VAR=VALUE\"" << std::endl;
                }
            }
            break;
            case QUIT:
            case EXIT:
                done = true;
            break;
            default:
                try {
                    shell.execute(token, "");
                }
                catch(...) {
                    std::cout << "INVALID COMMAND" << std::endl;
                }
        } 
    }

    std::cout << "\nExiting..." << std::endl;

    return 0;
}