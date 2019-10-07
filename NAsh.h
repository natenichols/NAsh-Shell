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
        bool active = true;

    public:
        NAsh() {};
        NAsh(char** envp) {
            //Populates Environment variables
            for(char** i = envp; *i != 0; i++) {
                char* envVar = strtok(*i, "=");
                char* envVal = strtok(NULL, " ");
                appendEnv(envVar, envVal);
            }
        }
        bool isActive() {return active;}
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