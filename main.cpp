#include<iostream>
#include "NAsh.h"

int main (int argc, char **argv, char **envp) {
    if(argc > 1 && argv[1] == std::string("-v")) {
        std::cout << "v0.0.1: Nathan Nichols and Andre Kurait: NAsh" << std::endl;
    }
    else {
        //run
    }
    
    return 0;
}