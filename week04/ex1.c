#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
int main()
{
	pid_t child1, child2;
	clock_t begin = clock();
	child1 = fork();
	
	if (child1 == 0) {
	    clock_t begin = clock();
	    printf("Child-process ID: %d, Parent ID: %d\n", getpid(), getppid());
	
	    printf("Time: %ld\n",clock() - begin);
	} else {
	    child2 = fork();
	  
	
	    if (child2 == 0) {
	        clock_t begin = clock();
		printf("Child-process ID: %d, Parent ID: %d\n", getpid(), getppid());
	        printf("Time: %ld\n", clock() - begin);
	    } else {
		printf("Parent-process ID: %d, Parent ID: %d\n", getpid(), getppid());

	        printf("Time: %ld\n", clock() - begin);
	    }
	}
	return 0;
}
