#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/resource.h>
#define MEMORY_ALLOCATION_SIZE (10 * 1024 * 1024) 

int main() {
    struct rusage usage;
    struct timeval start, end;

    getrusage(RUSAGE_SELF, &usage);
    start = usage.ru_stime;

    for (int i = 0; i < 10; i++) {
        char *mem = (char *)malloc(MEMORY_ALLOCATION_SIZE);
        if (mem == NULL) {
            perror("Memory allocation failed");
            return 1;
        }

        memset(mem, 0, MEMORY_ALLOCATION_SIZE);

        getrusage(RUSAGE_SELF, &usage);
        end = usage.ru_stime;

        printf("Memory usage (second %d):\n", i + 1);
        printf("System Time: %ld.%06ld seconds\n", end.tv_sec - start.tv_sec, end.tv_usec - start.tv_usec);
        

        free(mem); 
        
	start = end;
        sleep(1);
    }

    return 0;
}
