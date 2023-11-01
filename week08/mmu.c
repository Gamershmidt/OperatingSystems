#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/mman.h>
#include <string.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <stdbool.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/mman.h>
#include <string.h>
#include <sys/stat.h>

struct PTE {
    bool valid;     // 0 for not in RAM, 1 for in RAM
    int frame;      // Frame number of the page in RAM
    bool dirty;     // 0 for clean, 1 for dirty
    int referenced; // Number of times page is referenced
};
int numPages;
struct PTE *page_table;
void print_table(char* str) {
    printf("%s\n", str);
    for (int i = 0; i < numPages; ++i) {
        printf("Page %d ---> valid=%d, frame=%d, dirty=%d, referenced=%d\n",i,
               page_table[i].valid, page_table[i].frame, page_table[i].dirty,page_table[i].referenced);
    }
    printf("----------------------------\n");
}

int main(int argc, char *argv[]) {
    printf("\n\nPID mmu.c is %d\n\n", getpid());
    numPages = atoi(argv[1]);
    pid_t pid = atoi(argv[argc - 1]);

    mkdir("/tmp/ex2", 0777);

    FILE* fdfs = fopen("/tmp/ex2/pagetable", "w");
    fclose(fdfs);

    int fd = open("/tmp/ex2/pagetable", O_RDWR);
    if (fd == -1) {
        perror("open");
        return 1;
    }

    ftruncate(fd, numPages * sizeof (struct PTE));
    page_table = (struct PTE *)mmap(NULL, numPages * sizeof(struct PTE), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (page_table == MAP_FAILED) {
        perror("mmap");
        close(fd);
        return 1;
    }
    for (int i = 0; i < numPages; i++) {
        page_table[i].referenced = 0;
        page_table[i].valid = 0;
        page_table[i].dirty = false;
        page_table[i].frame = -1;
    }
    print_table("Initialised page table");
    for (int i = 2; i < argc - 1; i++) {
        char *mode_page = argv[i];
        int pageNum = atoi(&mode_page[1]);
        char mode = mode_page[0];
        if (mode == 'W') {
            page_table[pageNum].dirty = true;
            printf("Write request for page %d\n", pageNum);
        } else {
            printf("Read request for page %d\n", pageNum);
        }
        if (page_table[pageNum].valid == 0) {
            printf("It is not a valid page ---> page fault\n");
            printf("Ask pager to load it from disk (SIGUSR1 signal) and wait\n");
            page_table[pageNum].referenced = getpid();
            kill(pid, SIGUSR1);
            //printf("waiting......\n");
            kill(getpid(), SIGSTOP);
            printf("MMU resumed by SIGCONT signal from pager\n");

        } else {
            printf("It is valid page\n");
        }



        print_table("Page table");
    }

    close(fd);
    printf("Done all requests.\nMMU sends SIGUSR1 to the pager.\n");

    kill(pid, SIGUSR1);
    printf("MMU terminates.\n");
    munmap(page_table, numPages * sizeof(struct PTE));
    unlink("/tmp/ex2/pagetable");

    return 0;
}

