#include<iostream>
#include<unistd.h>
#include<string.h>
#include<sys/wait.h>
#include<map>

#ifndef NASH_H
#define NASH_H

class NAsh {
    private:
        std::map<std::string, char*> environmentVars;
    
    public:
        NAsh() {
            
        }

        void execute(char* cmd, char* args) {
            pid_t pid, new_pid;
            int status;

            //get all environment Vars associated with PATH
            std::string paths = environmentVars["PATH"];
            char * cstr = new char [paths.length()+1];
            strcpy (cstr, paths.c_str());

            //tokenize string at each :
            char* path = strtok(cstr, ":");

            //buffer for copying the path
            char pathWithCmd[256];

            while(path != NULL) {
                strcpy(pathWithCmd, path);
                strcat(pathWithCmd, "/");
                strcat(pathWithCmd, cmd);
                char* args[] = {pathWithCmd, *args, NULL};

                pid = fork();

                if(pid == 0) {
                    execvp(args[0], args);
                }
                else if(pid > 0) {

                    if( (new_pid = wait(&status)) < 0){
                        perror("wait");
                        _exit(1);
                    }
                }
                else {
                    //std::cout << "Fails" << std::endl;
                    //throw std::runtime_error("Invalid fork");
                }

                path = strtok(NULL, ":");
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
       // NAsh(int argc, char **argv, char **envp);
};
#endif //NASH_H