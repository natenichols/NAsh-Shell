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
        void printJobs();
};
#endif