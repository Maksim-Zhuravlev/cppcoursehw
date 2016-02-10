#include <stdio.h>
#include <mqueue.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/sem.h>
#include <sys/ipc.h>

int main(int argc, char *argv[]) {
	const int sems_count = 16;

	key_t key = ftok("/tmp/sem.temp", 1);
	int sem_d = semget(key, sems_count, IPC_CREAT | 0666);

	if (sem_d == -1) {
		perror("semget");
		exit(1);
	}

	struct sembuf sops[sems_count];
	for (int i = 0; i < sems_count; i++) {
	    sops[i].sem_num = i;
	    sops[i].sem_op = i;
	    sops[i].sem_flg = 0;
	}
	if (semop(sem_d, sops, sems_count) == -1) {
		perror("semop");
		exit(1);
	}

}