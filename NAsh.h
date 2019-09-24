#include<iostream>
#include<string>
#include<map>

#ifndef NASH_H
#define NASH_H

class NAsh {
    private:
        std::map<std::string, std::string> environmentVars;

        void populateEnv(char **envp) {
             for(char** i = envp; *i != 0; i++) {
                std::cout << "Environment Variable: " << *i << std::endl;
            }
        }

        void appendEnv(char* envVar, char* envVal) {
            environmentVars[envVar] = envVal;
        }

        void lsEnv() {
            for(auto x : environmentVars) {
                std::cout << x.first << "=" << x.second << std::endl;
            }
        }
    
    public:
        NAsh(int argc, char **argv, char **envp);
};
#endif //NASH_H