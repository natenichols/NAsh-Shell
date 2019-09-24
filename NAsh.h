#include<iostream>
#include<string>
#include<map>

#ifndef NASH_H
#define NASH_H

class NAsh {
    private:
        std::map<std::string, std::string> environmentVars;

        void populateEnv(char **envp) {
            for(int x = 0; envp[x] != nullptr; x++) {
                for(int y = 0; envp[x][y] != '\0'; y++) {
                    std::cout << envp[x][y];
                }
                std::cout << std::endl;
            }
        }
    
    public:
        NAsh(int argc, char **argv, char **envp);
};
#endif //NASH_H