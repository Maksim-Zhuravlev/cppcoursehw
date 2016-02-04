#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>

void termination_handler(int signum) {
	return;
} 

int main(int argc, char **argv) {
	const char path[] = "/home/box/pid";
	pid_t pid = getpid();
	FILE *pf = fopen(path, "wt");
	if (!pf) {
		perror("Cannot open file");		
		return 1;
	}
	fprintf(pf, "%ld", (long)pid);
	fclose(pf);
	struct sigaction new_action, old_action;
	new_action.sa_handler = termination_handler;
	sigemptyset(&new_action.sa_mask);
	new_action.sa_flags = 0;
	sigaction (SIGINT, &new_action, NULL);
	sigaction (SIGTERM, &new_action, NULL);
	while(1) {
		pause();
	}
	return 0;
}