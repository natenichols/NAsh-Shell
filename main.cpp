#include <iostream>
#include "NAsh.h"
#include <string.h>
#include <signal.h>
#include <vector>
#include<sstream>

std::vector<std::string> split(std::string str, char delim) {
  std::vector<std::string> tokens;
  std::stringstream ss(str); // Turn the string into a stream.
  std::string token;
 
  while(getline(ss, token, delim)) {
    tokens.push_back(token);
  }

  return tokens;
}

std::string& rtrim(std::string& str) {
    while(str[str.length()-1] == ' ') 
        str.erase(str.end()-1);
    return str;
}

std::string& ltrim(std::string& str) {
    while(str[0] == ' ') 
        str.erase(str.begin());
    return str;
}


std::string& trim(std::string& str) {
    return rtrim(ltrim(str));
}

void sigintHandler(int) 
{   
    char buf[FILENAME_MAX];
    getcwd(buf, FILENAME_MAX );
    std::cout << "\nNAsh@" << buf << "> " << std::flush;
} 

void welcome() {
    std::cout <<  " _________________" << std::endl;
    std::cout << "< Welcome to NAsh >" << std::endl;
    std::cout << " -----------------" << std::endl;
    std::cout << "\t\\   ^__^" << std::endl;
    std::cout << "\t \\  (oo)\\_______" << std::endl;
    std::cout << "\t    (__)\\       )\\/" << std::endl;
    std::cout << "\t\t||----w |" << std::endl;
    std::cout << "\t\t||     ||" << std::endl;
}

int main (int argc, char **argv) {
    if(argc > 1 && (strncmp(argv[1], "-v", 3) == 0 || strncmp(argv[1], "--version", 10) == 0)) {
        std::cout << "v0.0.1: Nathan Nichols and Andre Kurait: NAsh" << std::endl;
        return 0;
    }
    if (argc > 1 && (strncmp(argv[1], "-h", 3) == 0 ||  strncmp(argv[1], "--help", 7) == 0)) {
        std::cout << "Usage: nash <arguments> <options>" << std::endl << std::endl;
        std::cout << "Options:\n\t --help -h\t\t display help message, exits\n\t --version -v\t show version number, exits\n";
        std::cout << std::endl;
        return 0;
    }

    welcome();
    signal(SIGINT, sigintHandler);
    NAsh shell;

    while (shell.isActive() && !std::cin.eof()) {
        char buf[FILENAME_MAX];
        getcwd(buf, FILENAME_MAX );
        std::cout << "NAsh@" << buf << "> ";

        std::string strCMD;
        std::getline(std::cin, strCMD, '\n');
        if(strCMD.length() == 0) continue;
     
        //parse whole command line into  tokens
        std::vector<std::string> tokens = split(strCMD, '|');
        int pipe = -1;
        for(unsigned int i = 0; i < tokens.size(); i++) { 
                std::string token = tokens[i];
                std::string token_trim = trim(token);
                auto token_input = split(token_trim, '<');
                if(token_input.size() > 1) {
                    std::string inputFileName = trim(token_input[1]);
                    pipe = shell.createPipeFromFile(inputFileName);
                }
                auto token_output = split(token_input[0], '>');
                std::string outputFileName = (token_output.size() > 1) ? trim(token_output[1]) : "";
                auto token_final = split(token_output[0], ' ');
                if(outputFileName != "") {
                    shell.createFile(outputFileName);
                    pipe = shell.execInChild(token_final, pipe);
                    pipe = shell.overwriteFileFromPipe(outputFileName,pipe);
                }
                else if(i == tokens.size() - 1) shell.execInChild(token_final, pipe, STDOUT_FILENO);
                else pipe = shell.execInChild(token_final, pipe);

        }

        while(waitpid(-1, 0, WNOHANG) > 0) {
            // Wait for zombie processes]
        }

    }
    std::cout << "\nExiting..." << std::endl;

    return 0;
}