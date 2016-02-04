#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <vector>
#include <string>
#include <iostream>
#include <cstring>
#include <sys/wait.h>

std::string trim(const std::string& str, const std::string& whitespace = " \t") {
    const auto strBegin = str.find_first_not_of(whitespace);
    if (strBegin == std::string::npos)
        return ""; // no content

    const auto strEnd = str.find_last_not_of(whitespace);
    const auto strRange = strEnd - strBegin + 1;

    return str.substr(strBegin, strRange);
}

void splitString(std::vector<std::string> &out, const std::string input, const char * const delims) {    
    size_t oldPos = -1, newPos = 0;    
    while (newPos != std::string::npos) {
        oldPos++;
        newPos = input.find_first_of(delims, oldPos);        
        std::string part = trim(input.substr(oldPos, newPos != std::string::npos ? newPos - oldPos : newPos));        
        if (part.length() > 0) {
            out.push_back(part);
        }
        oldPos = newPos;
    }
    return;
}

char* const* makeExecArgs(std::vector<std::string> &v) {
    size_t argsCount = v.size();
    const char** res = (const char **)malloc((argsCount + 1) * sizeof(char*));
    for (size_t i = 0; i < argsCount; ++i) {
        res[i] = v[i].c_str();
    }
    res[argsCount] = NULL;
    return (char* const*) res;
}

void executeCommand(std::vector<std::string> command) {
    auto args = makeExecArgs(command);
    execvp(command[0].c_str(), args);
}

void makePipe(std::vector<std::vector<std::string>> commands, bool start = false) {
    if (start) {
        unlink("/home/box/result.out");        
        int fd = open("/home/box/result.out", O_WRONLY | O_CREAT, 0666);
        close(1);
        dup2(fd, 1);
        close(fd);
    }
    auto remainingCommands = commands.size();
    if (0 == remainingCommands) {
        return;
    } else if (1 == remainingCommands) {
        executeCommand(commands[0]);
    } else {
        //1+ commands
        int fd[2];
        pipe(fd);

        pid_t pid = fork();

        if (pid) {
            close(1);
            dup2(fd[1], 1);
            close(fd[0]);
            close(fd[1]);
            commands.pop_back();
            makePipe(commands);
        } else {
            close(0);
            dup2(fd[0], 0);
            close(fd[0]);
            close(fd[1]);
            executeCommand(commands[remainingCommands - 1]);
        }

    }    
}

int main(int argc, char **argv) {    
    char *buf = NULL;
    size_t len = 0;
    getline(&buf, &len, stdin);
    FILE *f = fopen ("/home/box/debug.txt", "wt");
    fprintf(f, "'%s'", buf);
    fclose(f);
    std::vector<std::string> commands(0);
    splitString(commands, buf, "|\n\r");
    std::vector<std::vector<std::string>> parsedCommands(0);
    for (auto i = commands.begin(); i != commands.end(); ++i) {
        std::vector<std::string> args(0);
        splitString(args, *i, " \t");
        parsedCommands.push_back(args);
    }

    makePipe(parsedCommands, true);
    return 0;
}