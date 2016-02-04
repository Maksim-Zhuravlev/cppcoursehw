#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>


int main(int argc, char **argv) {
	const char path_parent[] = "/home/box/pid_parent";
	const char path_child[] = "/home/box/pid_child";
	pid_t parent_pid = getpid();
	pid_t child_pid  = 0;
	FILE *pf = fopen(path_parent, "wt");
	FILE *cf = fopen(path_child, "wt");
	if (!pf || !cf) {
		perror("Cannot open files");
		fclose(pf);
		fclose(cf);
		return 1;
	}
	if (child_pid = fork()) {
		fprintf(pf, "%ld", (long) parent_pid);
		fprintf(cf, "%ld", (long) child_pid);
		fclose(pf);
		fclose(cf);
		int status = 0;
		waitpid(child_pid, &status, 0);
	} else {
		pause();
	}
	return 0;
}