#include"NAsh.h"

std::map<int, std::pair<int, std::string>>*  g_jobs;

std::string getPath(std::string s) {
    return s;
}
void NAsh::printJobs() {
   for (auto it : jobs) {
        std::cout << "[" << it.second.first << "] " << it.first  << " " << it.second.second << std::endl;
    }
}
void sigchild_handler(int sig)
{
    // int saved_errno = errno;
    // while (waitpid((pid_t)(-1), 0, WNOHANG) > 0) {}
    // errno = saved_errno;

    // if(g_jobs->find(pid) != g_jobs->end())
    //     std::cout << "[" <<  (*g_jobs)[pid].first << "] " << pid  << " " << (*g_jobs)[pid].second << std::endl;
    // exit(1);
   
}

int NAsh::execInChild(std::vector<std::string> cmd, int readPipe) {

            g_jobs = &jobs;
            signal(SIGCHLD,sigchild_handler);

            if(cmd.size() == 0 || cmd[0].length() == 0) return -1;
            if(cmd[0] == "exit" || cmd[0] == "quit") {
                    this->active = false;
                    return -1;
            }
            
            if(cmd[0][0] != '/') {
                cmd[0] = getPath(cmd[0]);
            }

            int pipefd[2], status;

            if(pipe(pipefd) == -1) {
                std::cout << "Pipe failed to build" << std::endl;
                exit(1);
            }

            pid_t pid = fork();
            
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
                    
                if(cmd[0] == "jobs") {
                    printJobs();
                    exit(0);
                }
                execvp(args[0], args);
                exit(0);
            }
            close(pipefd[1]);
            if(readPipe != -1) close(readPipe);

            if (!background && (waitpid(pid, &status, 0)) == -1) {
                fprintf(stderr, "Process encountered an error. ERROR%d", errno);
                exit(1);
            } else if(background) {
                std::cout << "[" << processCounter << "] " << pid << " running in background" << std::endl;
            }
            return pipefd[0];
}

void NAsh::printFromPipe(int pipe) {
    if(pipe == -1) return;
    int pid, status;
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

    if ((waitpid(pid, &status, 0)) == -1) {
        fprintf(stderr, "Print Process encountered an error. ERROR%d", errno);
        exit(1);
    }
}
