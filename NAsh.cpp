#include"NAsh.h"

void printFinishedBackground(int sig);
std::map<int, std::pair<int, std::string>>* g_jobs;

NAsh::NAsh() {
    //Populates Environment variables
    active = true;
    processCounter = 0;

    g_jobs = &jobs;
    struct sigaction sa;
    sa.sa_handler = &printFinishedBackground;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    sigaction(SIGCHLD, &sa, NULL);
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
    if(!finishedPIDS.empty()) {
        char buf[FILENAME_MAX];
        getcwd(buf, FILENAME_MAX );
        std::cout << "NAsh@" << buf << "> " << std::flush;
    }
}


int NAsh::execInChild(std::vector<std::string> cmd, int readPipe, int writePipe) {
            if(cmd[0] == std::string("cd")) {
                if(cmd.size() == 1) {
                    char* dir = getenv("HOME");
                    if(dir == nullptr) {
                        std::cout << "NAsh: cd: HOME not set\n";
                    }
                    else if(chdir(dir)) {
                        std::cout << "NAsh: cd: " << dir << ": No such file or directory" << std::endl;
                    }
                }
                else
                    if(chdir(cmd[1].c_str())) std::cout << "NAsh: cd: " << cmd[1] <<": No such file or directory" << std::endl;
                return -1;
            }
            if(cmd[0] == "set") {
                char* envVar = new char[cmd[1].length()+1];
                strcpy(envVar, cmd[1].c_str());
                putenv(envVar);
                return -1;
            }

            if(cmd.size() == 0 || cmd[0].length() == 0) return -1;
            if(cmd[0] == "exit" || cmd[0] == "quit") {
                    this->active = false;
                    return -1;
            }

            int pipefd[2];
            if(writePipe != -1) {
                pipefd[1] = writePipe;
                pipefd[0] = -1;
            }
            else if(pipe(pipefd) == -1) {
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
                for(size_t i = 0; i < cmd.size(); i++) {
                    strCMD += cmd[i] + ((i != cmd.size() - 1) ? " " : "");
                }
                jobs.insert({pid, {++processCounter, strCMD}});
            }
            if(pid == 0) {
                // Child
                if(readPipe != -1) {
                    dup2(readPipe, STDIN_FILENO);
                    close(readPipe);
                } else if (background) {
                    close(STDIN_FILENO);
                }
                if(pipefd[1] != STDOUT_FILENO) { 
                    dup2(pipefd[1], STDOUT_FILENO);
                    if(pipefd[0] != -1) close(pipefd[0]);
                    close(pipefd[1]);
                }


                if(cmd[0] == "kill") {
                    int givenPID = stoi(cmd[2]);
                    for(auto it : jobs) {
                        if(it.second.first == givenPID) {
                            kill(it.first, stoi(cmd[1]));
                            exit(0);
                        }
                    }
                    std::cout << "Invalid JobID Specified\n";
                    exit(1);
                }
                char** args = new char*[cmd.size()+1];
                for(size_t x = 0; x < cmd.size(); x++) 
                    args[x] = (char*)cmd[x].c_str();
                args[cmd.size()] = NULL;
                if(cmd[0] == "jobs") {
                    printJobs();
                    exit(0);
                }

                execvp(args[0], args);
                std::cout << "NAsh: " << cmd[0] << ": " << strerror(errno) << std::endl;
                exit(errno);
            }
            if(pipefd[1] != STDOUT_FILENO) close(pipefd[1]);

            if (background) {
                std::cout << "[" << processCounter << "] " << pid << " running in background" << std::endl;
            } else {
                waitpid(pid, &status, 0);
            }
            
            return pipefd[0];
}

int NAsh::createPipeFromFile(std::string fileName) {
    std::ifstream inFile(fileName);
    std::string line;

    int pipefd[2];
    if(pipe(pipefd) == -1) {
        std::cout << "Pipe failed" << std::endl;
        return -1;
    }
    
    while(std::getline(inFile, line)) {
        line.push_back('\n');
        write(pipefd[1], line.c_str(), line.size());
    }

    close(pipefd[1]);
    return pipefd[0];
}
int NAsh::overwriteFileFromPipe(std::string fileName, int readPipe) {
    if(readPipe == -1 || fileName == "") {
        std::cout << "Invalid Command\n";
        return -1;
    }

    pid_t pid = fork();
    int status;
    if(pid == 0) {
        // Child
        freopen (fileName.c_str(),"w",stdout);

        int bytes; 
        char buffer[BSIZE];
        while ( (bytes = read(readPipe, buffer, BSIZE)) > 0) {
            write(STDOUT_FILENO, buffer, bytes);
        }

        fclose(stdout);
        close(readPipe);
        exit(0);
    }
    close(readPipe);
    waitpid(pid, &status, 0);
    return -1;
}
bool NAsh::createFile(std::string fileName) {
    std::ofstream myfile;
    myfile.open(fileName);
    return true;
}