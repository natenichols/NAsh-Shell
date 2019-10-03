#include<iostream>
#include<unistd.h>
#include<string.h>
#include<sys/wait.h>
#include<unordered_map>

#ifndef NASH_H
#define NASH_H

class NAsh {
    private:
        std::unordered_map<std::string, char*> environmentVars;
        int BSIZE = 256;
    
    public:
        NAsh() {}
        void printFromPipe(int pipe);
        void execute(char* cmd, char* args);
        int execInChild(std::string cmd, int readPipe = -1);
        void appendEnv(char* envVar, char* envVal) {
            environmentVars[envVar] = envVal;
        }
        void lsEnv() {
            for(auto x : environmentVars) {
                std::cout << x.first << "=" << x.second << std::endl; 
            }
        }
};
#endif