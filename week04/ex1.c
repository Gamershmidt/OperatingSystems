#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
int main()
{
	pid_t child1, child2;
	
	child1 = fork();
	clock_t begin = clock();
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
