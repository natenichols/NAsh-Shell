#ifndef NASH_H
#define NASH_H

#include<iostream>
#include<vector>
#include<unistd.h>
#include<string.h>
#include<sys/wait.h>
#include<unordered_map>
#include<map>
#include<signal.h>
#include<sstream>

class NAsh {
    private:
        std::unordered_map<std::string, char*> environmentVars;
        std::map<int, std::pair<int, std::string>> jobs; 
        int BSIZE = 256;
        bool active;
        int processCounter;

    public:
        NAsh() {active = true; processCounter = 0;};
        NAsh(char** envp);
        bool isActive() {return active;}
        void printFromPipe(int pipe);
        void execute(char* cmd, char* args);
        int execInChild(std::vector<std::string> cmd, int readPipe = -1);
        void appendEnv(char* envVar, char* envVal) {
            environmentVars[envVar] = envVal;
        }
        void lsEnv() {
            for(auto x : environmentVars) {
                std::cout << x.first << "=" << x.second << std::endl; 
            }
        }
        void printJobs();
};
#endif