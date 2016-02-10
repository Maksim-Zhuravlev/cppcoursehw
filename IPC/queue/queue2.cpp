#include <stdio.h>
#include <mqueue.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <mqueue.h>


/* max length of a message (just for this process) */
#define MAX_MSG_LEN     10000


int main(int argc, char *argv[]) {
    mqd_t msgq_id;
    char msgcontent[MAX_MSG_LEN];
    int msgsz;
    unsigned int sender;
    struct mq_attr msgq_attr;
    
    
    /* opening the queue        --  mq_open() */
    msgq_id = mq_open("/test.mq", O_RDWR | O_CREAT);
    if (msgq_id == (mqd_t)-1) {
        perror("In mq_open()");
        exit(1);
    }

    /* getting the attributes from the queue        --  mq_getattr() */
    mq_getattr(msgq_id, &msgq_attr);
    printf("Queue \"%s\":\n\t- stores at most %ld messages\n\t- large at most %ld bytes each\n\t- currently holds %ld messages\n", "/test.mq", msgq_attr.mq_maxmsg, msgq_attr.mq_msgsize, msgq_attr.mq_curmsgs);

    /* getting a message */
    msgsz = mq_receive(msgq_id, msgcontent, MAX_MSG_LEN, &sender);
    if (msgsz == -1) {
        perror("In mq_receive()");
        exit(1);
    }
    printf("Received message (%d bytes) from %d: %s\n", msgsz, sender, msgcontent);
    
    /* closing the queue    --  mq_close() */
    mq_close(msgq_id);

    FILE *f = fopen("/home/box/message.txt", "w+t");

    if (!f) {
        perror("fopen");
        exit(1);
    }

    if (fprintf(f, "%s", msgcontent) == -1) {
        perror("fprintf");
        fclose(f);
        exit(1);

    }

    fclose(f);
    
    
    return 0;
}