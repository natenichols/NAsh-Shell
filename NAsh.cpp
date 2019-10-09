#include"NAsh.h"

void printFinishedBackground(int sig);

std::map<int, std::pair<int, std::string>>* g_jobs;

NAsh::NAsh(char** envp) {
    //Populates Environment variables
    for(char** i = envp; *i != 0; i++) {
        char* envVar = strtok(*i, "=");
        char* envVal = strtok(NULL, " ");
        appendEnv(envVar, envVal);
    }
    active = true;
    processCounter = 0;

    g_jobs = &jobs;
    struct sigaction sa;
    sa.sa_handler = &printFinishedBackground;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sigaction(SIGCHLD, &sa, NULL);
}
std::string getPath(std::string s) {
    return s;
}
void NAsh::printJobs() {
   for (auto it : jobs) {
        std::cout << "[" << it.second.first << "] " << it.first  << " " << it.second.second << std::endl;
    }
}

bool isRunning(pid_t pid) {   
    if (0 == kill(pid, 0))
        return true;
    return false;
}

#include<unordered_set>
void printFinishedBackground(int sig) {
    while(waitpid(-1, 0, WNOHANG) > 0) {
        // Wait for zombie processes]
    }

    std::unordered_set<int> finishedPIDS;
    for (auto it : (*g_jobs)) {
        if(!isRunning(it.first)) {
            if(finishedPIDS.empty()) std::cout << std::endl;
            std::cout << "[" << it.second.first << "] " << it.first  << " finished " << it.second.second << std::endl;
            finishedPIDS.insert(it.first);
        }
    }
    for(auto ended : finishedPIDS) {
        g_jobs->erase(ended);
    }
}


int NAsh::execInChild(std::vector<std::string> cmd, int readPipe) {
            if(cmd[0] == std::string("cd")) {
                if(cmd.size() == 1) 
                    chdir(environmentVars["HOME"]);
                else
                    chdir(cmd[1].c_str());
            }

            if(cmd.size() == 0 || cmd[0].length() == 0) return -1;
            if(cmd[0] == "exit" || cmd[0] == "quit") {
                    this->active = false;
                    return -1;
            }
            
            if(cmd[0][0] != '/') {
                cmd[0] = getPath(cmd[0]);
            }

            int pipefd[2];

            if(pipe(pipefd) == -1) {
                std::cout << "Pipe failed to build" << std::endl;
                exit(1);
            }

            pid_t pid = fork();
            int status;

            bool background = false;
            if (cmd[cmd.size()-1] == "&") {
                background = true;
                cmd.pop_back();

                std::string strCMD = "";
                for(int i = 0; i < cmd.size(); i++) {
                    strCMD += cmd[i] + ((i != cmd.size() - 1) ? " " : "");
                }
                jobs.insert({pid, {++processCounter, strCMD}});
            }

            if(pid == 0) {
                // Child
                if(readPipe != -1) {
                    dup2(readPipe, STDIN_FILENO);
                    close(readPipe);
                }
                dup2(pipefd[1], STDOUT_FILENO);

                close(pipefd[0]);
                close(pipefd[1]);

                char** args = new char*[cmd.size()+1];
                for(size_t x = 0; x < cmd.size(); x++) 
                    args[x] = (char*)cmd[x].c_str();
                args[cmd.size()] = NULL;
                if(cmd[0] == "jobs") {
                    printJobs();
                    exit(0);
                }

                execvp(args[0], args);
                std::cout << "Error is: " << std::strerror(errno) << std::endl;
                exit(errno);
            }
            close(pipefd[1]);

            if (background) {
                std::cout << "[" << processCounter << "] " << pid << " running in background" << std::endl;
            } else {
                waitpid(pid, &status, 0);
            }
            
            return pipefd[0];
}

void NAsh::printFromPipe(int pipe) {
    int status;
    if(pipe == -1) return;
    int pid;
    if((pid = fork()) == 0) {
        // Child
        dup2(pipe, STDIN_FILENO);
        close(pipe);
        
        char buffer[BSIZE];
        int bytes;
        while ( (bytes = read(STDIN_FILENO, buffer, BSIZE)) > 0) {
            write(STDOUT_FILENO, buffer, bytes);
        }
        exit(0);
    }
   waitpid(pid, &status, 0);
}
