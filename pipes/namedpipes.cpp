#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>


int main(int argc, char **argv) {
	const char path_in[] = "/home/box/in.fifo";
	const char path_out[] = "/home/box/out.fifo";
	int rc1 = mkfifo(path_in, 0666);
	int rc2 = mkfifo(path_out, 0666);
	if (rc1 || rc2) {
		perror("Error: Cannot create pipes");
		exit(1);
	}
	int fd_in = 0, fd_out = 0;
	fd_in = open(path_in, O_RDONLY);
	fd_out = open(path_out, O_WRONLY);
	if (fd_in > 0 && fd_out > 0) {
		int len = 0;
		char buf[512];
		do {
			len = read(fd_in, &buf, sizeof(buf));
			write(fd_out, &buf, len);		
		} while (len > 0);
	} else {
		perror("Error: Cannot open pipe");
	}	
	close(fd_in);
	close(fd_out);

	return 0;
}