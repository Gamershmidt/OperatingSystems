#include<stdio.h>
#include<stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
pid_t pid;

void handler(int signum) {
    if (signum == SIGINT) {
        kill(pid, SIGTERM);
        exit(EXIT_SUCCESS);
    }
}
int main() {
    signal(SIGINT, handler);
    FILE *file = fopen("/var/run/agent.pid", "r");
    if (file != NULL) {
       	
        
	if (fscanf(file, "%d", &pid) == 1) {
            printf("Agent found: %d \n", pid);
	} else {
	        printf("Error: Unable to read PID from the file.\n");
	}
        	fclose(file);
    } else {
        printf("Error: No agent found.\n");
    }
    	
    char command[100];

    while (1) {
        printf("Choose a command {\"read\", \"exit\", \"stop\", \"continue\"}: ");
        
        if (fgets(command, sizeof(command), stdin) != NULL) {
            size_t len = strlen(command);
            if (len > 0 && command[len - 1] == '\n') {
                command[len - 1] = '\0';
            }

            if (strcmp(command, "read") == 0) {
                kill(pid, SIGUSR1);
                sleep(2);
            } else if (strcmp(command, "exit") == 0) {
                kill(pid, SIGUSR2);
                break; 
            } else if (strcmp(command, "stop") == 0) {
                kill(pid, SIGSTOP);
            } else if (strcmp(command, "continue") == 0) {
                kill(pid, SIGCONT);
            } else {
                printf("Invalid command. Please choose from {\"read\", \"exit\", \"stop\", \"continue\"}.\n");
            }
        } else {
            printf("Error reading input. Exiting.\n");
            break;
        }
        
    }
    return 0;
}

