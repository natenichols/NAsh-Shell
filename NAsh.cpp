#include"NAsh.h"

 void NAsh::execute(char* cmd, char* args) {     
            printFromPipe(execInChild("grep main", execInChild("ls")));
            std::cout<<std::endl;
}

int NAsh::execInChild(std::string cmd, int readPipe) {
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
                
                if(cmd == "ls") {
                    char* args[2] = {"ls", NULL};
                    execv("/bin/ls", args);
                }
                
                char* args[3] = {"grep", "main", NULL};
                execv("/usr/bin/grep", args);
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
