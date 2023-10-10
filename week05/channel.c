#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAX_SIZE 1024

int main(void) {
	int fd[2];
	pid_t subscriber;
	if(pipe(fd) == -1) exit(EXIT_FAILURE);
	subscriber = fork();
	if(subscriber == 0) {
		close(fd[1]);
		char message[MAX_SIZE];
		ssize_t input = read(fd[0], message, MAX_SIZE);
		write(STDOUT_FILENO, message, input);
		close(fd[0]);
		exit(EXIT_SUCCESS);
	} else {
		close(fd[0]);
		char message[MAX_SIZE];
		ssize_t input = read(STDIN_FILENO, message, MAX_SIZE);
		ssize_t out = write(fd[1] , message, input);
		close(fd[1]);
		wait(NULL);
		exit(EXIT_SUCCESS);
	}
	return 0;
}
