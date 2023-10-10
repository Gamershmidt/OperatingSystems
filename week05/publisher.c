#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>
int main(int argc, char *argv[]) {
  
    int n = atoi(argv[1]);
    char message[1024];
    fgets(message, 1024, stdin);
    for (int i = 0; i < n; i++) {
        pid_t pid = fork();

        if (pid == -1) {
            perror("fork");
            return 1;
        }

        if (pid == 0) {
            char path[100];
            snprintf(path, 100, "/tmp/ex1/s%d", i+1);
            mkfifo(path, 0666);
            int fd;
	    fd = open(path, O_WRONLY);
	    if (fd == -1) {
	      perror("open pub");
	      exit(EXIT_FAILURE);
	    }
	    write(fd, message, strlen(message) + 1);
	    close(fd);
            return 0;
        }
    }


    for (int i = 0; i < n; i++) {
        wait(NULL);
    }

    return 0;
}

