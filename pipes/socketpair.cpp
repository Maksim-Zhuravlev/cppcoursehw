#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>


int main(int argc, char **argv) {
    int fd[2];
    static const int parentsocket = 0;
    static const int childsocket = 1;
    pid_t pid;
    
    socketpair(PF_LOCAL, SOCK_STREAM, 0, fd);

    pid = fork();
    if (pid == 0) { /* 2.1 if fork returned zero, you are the child */
        close(fd[parentsocket]); /* Close the parent file descriptor */
        pause();
    } else { /* 2.2 ... you are the parent */
        close(fd[childsocket]); /* Close the child file descriptor */
        pause();
    }
	return 0;
}