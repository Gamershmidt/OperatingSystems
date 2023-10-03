#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
int main()
{
	pid_t child1, child2;
	child1 = fork();
	if (child1 == -1) {
	    perror("fork");
	    exit(1);
	}
	clock_t begin1 = clock();
	if (child1 == 0) {
	    printf("Child-process ID: %d, Parent ID: %d\n", getpid(), getppid());
	    clock_t end1 = clock();
	    printf("Time: %ld\n",end1 - begin1);
	    exit(0);
	} else {
	    child2 = fork();
	    if (child1 == -1) {
		perror("fork");
	        exit(1);
	    }
	    clock_t begin2 = clock();

	    if (child2 == 0) {
		printf("Child-process ID: %d, Parent ID: %d\n", getpid(), getppid());
		clock_t end2 = clock();
	        printf("Time: %ld\n", end2 - begin2);
	        exit(0);
	    } else {
	    	wait(NULL);
	    	wait(NULL);
		printf("Parent-process ID: %d, Parent ID: %d\n", getpid(), getppid());
		clock_t end1 = clock();
	        printf("Time: %ld\n", end1 - begin1);
	    }
	}
	return 0;
}
