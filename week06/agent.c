#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define SIZE 1024

void handler(int signum) {
    switch(signum){
    case SIGUSR1:
        FILE *file2 = fopen("text.txt", "r");
        char buffer[SIZE];
        fgets(buffer, SIZE, file2);
        printf("%s", buffer);
        break;
    case SIGUSR2:
        printf("Process terminating...\n");
        exit(EXIT_SUCCESS);
        break;
    default:
        break;
    }
}

int main() {
    FILE *file = fopen("/var/run/agent.pid", "w+");
    fprintf(file, "%d", getpid());
    FILE *file2 = fopen("text.txt", "r");
    char buffer[SIZE];
    fgets(buffer, SIZE, file2);
    printf("%s", buffer);
    fclose(file);
    signal(SIGUSR1, handler);
    signal(SIGUSR2, handler);
    while (1) {
        sleep(1);
    }
    remove("/var/run/agent.pid");
    return 0;
}
