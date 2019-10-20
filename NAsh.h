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
#include <stdlib.h>
#include <unordered_set>
#include<sstream>
#include<fstream>

class NAsh {
    private:
        std::map<int, std::pair<int, std::string>> jobs; 
        int BSIZE = 256;
        bool active;
        int processCounter;
        void printJobs();
    public:
        NAsh();
        bool isActive() {return active;}
        int execInChild(std::vector<std::string> cmd, int readPipe = -1, int writePipe = -1);
        int createPipeFromFile(std::string fileName);
        int overwriteFileFromPipe(std::string fileName, int readPipe);
        bool createFile(std::string fileName);

};
#endif