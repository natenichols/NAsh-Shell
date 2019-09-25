#include<iostream>
#include<string>
#include<map>

#ifndef NASH_H
#define NASH_H

class NAsh {
    private:
        std::map<std::string, std::string> environmentVars;
    
    public:
        NAsh() {
            
        }

        void execute(char* cmd) {
            
        }

        void appendEnv(char* envVar, char* envVal) {
            environmentVars[envVar] = envVal;
        }

        void lsEnv() {
            for(auto x : environmentVars) {
                std::cout << x.first << "=" << x.second << std::endl;
            }
        }
       // NAsh(int argc, char **argv, char **envp);
};
#endif //NASH_H