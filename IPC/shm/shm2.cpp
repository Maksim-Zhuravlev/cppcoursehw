#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>

#define SHM_SIZE 1048576

int main(int argc, char *argv[])
{
    int shmfd;
    int shared_seg_size = SHM_SIZE;
    
        
    shmfd = shm_open("/test.shm", O_CREAT | 0666, S_IRWXU | S_IRWXG);
    if (shmfd < 0) {
        perror("In shm_open()");
        exit(1);
    }
    fprintf(stderr, "Created shared memory object %s\n", "/test.shm");
    
    /* adjusting mapped file size (make room for the whole segment to map)      --  ftruncate() */
    ftruncate(shmfd, shared_seg_size);

    /* requesting the shared segment    --  mmap() */    
    char *data = (char *)mmap(NULL, shared_seg_size, PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0);
    if (data == NULL) {
        perror("In mmap()");
        exit(1);
    }
    fprintf(stderr, "Shared memory segment allocated correctly (%d bytes).\n", shared_seg_size);

    memset(data, 13, shared_seg_size);

    pause();

    return 0;
}