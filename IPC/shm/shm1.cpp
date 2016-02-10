#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#define SHM_SIZE 1048576

int main(int argc, char *argv[])
{
    key_t key;
    int shmid;
    char *data;
    int mode;
    
    if ((key = ftok("/tmp/mem.temp", 1)) == -1) {
        perror("ftok");
        exit(1);
    }
    
    if ((shmid = shmget(key, SHM_SIZE, 0666 | IPC_CREAT)) == -1) {
        perror("shmget");
        exit(1);
    }
    
    data = shmat(shmid, 0, 0);
    if (data == (char *)(-1)) {
        perror("shmat");
        exit(1);
    }

    memset(data, 42, SHM_SIZE);

    pause();

    return 0;
}