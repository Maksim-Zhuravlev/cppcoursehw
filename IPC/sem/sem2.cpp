#include <stdio.h>
#include <mqueue.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>
#include <pthread.h>
#include <semaphore.h>

int main(int argc, char *argv[]) {
	sem_t *sem = sem_open("/test.sem", O_CREAT, 0666, 66);
	if (sem == SEM_FAILED) {
		perror("sem_open");
		exit(1);
	}

	return 0;

}
