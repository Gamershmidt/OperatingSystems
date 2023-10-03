#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

int main(int argc, char **argv) {
    int n = atoi(argv[1]);
    for(int i = 0; i < n; i++) {
    	fork();
    	sleep(5);
    }
    return 0;
}

