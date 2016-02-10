#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

struct message {
    long mtype;
    char mtext[80];
};

int main(void)
{
    struct message buf = {0};
    int msqid;
    key_t key;

    if ((key = ftok("/tmp/msg.temp", 1)) == -1) {  /* same key as kirk.c */
        perror("ftok");
        exit(1);
    }

    if ((msqid = msgget(key, IPC_CREAT | 0666)) == -1) { /* connect to the queue */
        perror("msgget");
        exit(1);
    }

    
    if (msgrcv(msqid, &buf, sizeof buf.mtext, 0, 0) == -1) {
        perror("msgrcv");
        exit(1);
    }
    printf("Got message: \"%s\"\n", buf.mtext);

    FILE *f = fopen("/home/box/message.txt", "w+t");

    if (!f) {
        perror("fopen");
        exit(1);
    }

    if (fprintf(f, "%s", buf.mtext) == -1) {
        perror("fprintf");
        fclose(f);
        exit(1);

    }

    fclose(f);

    return 0;
}