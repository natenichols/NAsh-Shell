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
        char** envs;

        int BSIZE = 256;
    
    public:
        NAsh(char** env) {
            envs = env;
        }

        void execute(char* cmd, char* args) {
            pid_t pid;
            int pipefd[2];
            int status;

            //get all environment Vars associated with PATH
            std::string paths = environmentVars["PATH"];
            char * cstr = new char [paths.length()+1];
            strcpy (cstr, paths.c_str());

            //tokenize string at each :
            char* path = strtok(cstr, ":");

            //buffer for copying the path
            char pathWithCmd[BSIZE];
            char buffer[BSIZE];

            if(pipe(pipefd) == -1) {
                std::cout << "Pipe failed to build" << std::endl;
                _exit(-1);
            }

            pid = fork();

            if(pid == 0) {
                while(path != NULL) {
                    strcpy(pathWithCmd, path);
                    strcat(pathWithCmd, "/");
                    strcat(pathWithCmd, cmd);
                    char* args[] = {pathWithCmd, *args, 0};

                    dup2 (pipefd[1], STDOUT_FILENO);
                    close(pipefd[1]);
                    close(pipefd[0]);
                    int res = execvpe(args[0], args, envs);

                    if(res > 0) return;

                    path = strtok(NULL, ":");
                }
            }
            else if(pid > 0) {
                close(pipefd[1]);
                wait(&status);
                while(read(pipefd[0], buffer, BSIZE) != NULL);
                    std::cout << buffer;
                wait(NULL);
            }
            else {
               // std::cout << "Fails" << std::endl;
                throw std::runtime_error("Invalid fork");
            }

            std::cout << std::endl;
            
            delete cstr;
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