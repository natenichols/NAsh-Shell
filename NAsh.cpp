#include"NAsh.h"

//  void NAsh::execute(char* cmd, char* args) {     
//             printFromPipe(execInChild("grep main", execInChild("ls")));
//             std::cout<<std::endl;
// }

int NAsh::execInChild(std::vector<std::string> cmd, int readPipe) {
            if(cmd[0] == "exit" || cmd[0] == "quit") {
                    this->active = false;
                    return -1;
            }
            pid_t pid;
            int pipefd[2], status;

            if(pipe(pipefd) == -1) {
                std::cout << "Pipe failed to build" << std::endl;
                exit(1);
            }

            if((pid = fork()) == 0) {
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
                    
                
                execv(args[0], args);
                
                // char* args[3] = {"grep", "main", NULL};
                // execv("/usr/bin/grep", args);
                exit(0);
            }
            close(pipefd[1]);
            if(readPipe != -1) close(readPipe);

            if ((waitpid(pid, &status, 0)) == -1) {
                fprintf(stderr, "Process encountered an error. ERROR%d", errno);
                exit(1);
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
