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
#include <limits.h>
#define MAX_FRAMES 300
#define MAX_PAGES 300
#define MAX_STR 8
#define AGING_COUNTER_BITS 8
struct PTE {
    bool valid;
    int frame;
    bool dirty;
    int referenced;
};

struct PTE *page_table;


int num_pages;          // Number of pages
int num_frames;         // Number of frames
char **RAM;               // RAM array
char **disk;        // Disk array
int disk_access_count;  // Counter for disk accesses
char algorithm[20];
void print_pages(char* str) {
    printf("%s\n", str);
    for(int i = 0; i < num_pages; i++) {
        printf("Page %d ---> valid=%d, frame=%d, dirty=%d, referenced=%d\n", i, page_table[i].valid, page_table[i].frame, page_table[i].dirty,page_table[i].referenced);
    }
    printf("------------------------------------");
}
void print_frames() {
    printf("RAM array\n");
    for (int i = 0; i < num_frames; i++) {

        printf("Frame %d --> %s\n", i, RAM[i]);
    }

}
void print_disk() {
    printf("Initialised disk\nDisk array\n");
    for (int i = 0; i < num_pages; i++) {

        printf("Page %d --> %s\n", i, disk[i]);
    }

}
// Random page replacement
int Random(struct PTE* page_table, int amount){
    return rand() % amount;
};
// NFU page replacement
int nfu(struct PTE* page_table, int page_table_size) {
    int min_referenced = INT_MAX;
    int replace_page = -1;

    for (int i = 0; i < page_table_size; i++) {
        if (page_table[i].valid) {
            if (page_table[i].referenced < min_referenced) {
                min_referenced = page_table[i].referenced;
                replace_page = i;
            }
        }
    }

    return replace_page;
}
// Aging page replacement
int aging(struct PTE* page_table, int num_entries) {
    // Initialize an array to store aging counters for each page
    int aging_counters[num_entries];

    // Initialize the aging counters to zero
    for (int i = 0; i < num_entries; i++) {
        aging_counters[i] = 0;
    }

    // Simulate aging by shifting the counters right and updating referenced bit
    for (int i = 0; i < num_entries; i++) {
        if (page_table[i].valid) {
            aging_counters[i] >>= 1; // Shift aging counter to the right
            if (page_table[i].referenced) {
                aging_counters[i] |= (1 << (AGING_COUNTER_BITS - 1)); // Set the leftmost bit if referenced
                page_table[i].referenced = 0; // Reset the referenced bit
            }
        }
    }

    int min_aging_counter = aging_counters[0];
    int min_aging_index = 0;

    // Find the page with the smallest aging counter
    for (int i = 1; i < num_entries; i++) {
        if (aging_counters[i] < min_aging_counter) {
            min_aging_counter = aging_counters[i];
            min_aging_index = i;
        }
    }

    return min_aging_index;
}

// Signal handler for SIGUSR1
void handler(int signo) {
    sleep(1);
    int *free_frames = (int *)malloc(num_frames*sizeof(int));
    for (int i = 0; i < num_frames; i++) {
        free_frames[i] = -1;
    }
    for (int i = 0; i < num_pages; i++) {
        if(page_table[i].valid) {
            free_frames[page_table[i].frame] = i;
        }
    }
    for (int i = 0; i < num_pages; i++) {
        if (page_table[i].referenced != 0) {
            printf("A disk access request from MMU Process (pid=%d)\n",page_table[i].referenced);
            printf("Page %d is referenced\n", i);
            if (!page_table[i].valid) {
                int free_frame = -1;
                for (int j = 0; j < num_frames; j++) {
                    if (free_frames[j] == -1) {
                        free_frame = j;
                        break;
                    }
                }

                if (free_frame != -1) {
                    printf("We can allocate it to free frame %d\n", free_frame);
                    free_frames[free_frame] = i;
                    page_table[i].frame = free_frame;
                    page_table[i].valid = true;
                    page_table[i].dirty = false;
                    pid_t pid = page_table[i].referenced;
                    page_table[i].referenced = 0;
                    printf("Copy data prom a disk (page=%d) to RAM (frame=%d)\n", i, free_frame);
                    printf("\n%s %s %s\n", disk[0],disk[1],disk[2]);
                    strcpy(RAM[free_frame], disk[i]);
                    disk_access_count++;
                    printf("disk accesses is %d so far\n", disk_access_count);
                    print_frames();
                    printf("Resume MMU process %d\n", pid);
                    kill(pid, SIGCONT);
                    // SIGCONT
                } else {
                    printf("We do not have free frames in RAM\n");
                    int victim_frame = 0, victim_page = 0;
                    if(strcmp(algorithm, "random")) {
                        victim_frame = Random(page_table, num_frames);
                        victim_page = free_frames[victim_frame];
                    } else if (strcmp(algorithm, "nfu")) {
                        victim_page = nfu(page_table, num_pages);
                    } else if (strcmp(algorithm, "aging")) {
                        victim_page = aging(page_table, num_pages);
                    }
                    victim_frame = page_table[victim_page].frame;
                    printf("Chose a random victim page %d\n", victim_page);
                    printf("Replace/Evict it with page %d to be allocated to frame %d\n", i, victim_frame);


                    if (page_table[victim_page].dirty) {
                        strcpy(disk[victim_frame],RAM[page_table[victim_page].frame] );
                        disk_access_count++;
                        printf("disk accesses is %d so far\n", disk_access_count);
                        strcpy(RAM[victim_frame], disk[i]);
                        page_table[victim_page].valid = false;
                        page_table[victim_page].frame = -1;
                        page_table[victim_page].dirty = false;
                        free_frames[victim_frame] = i;
                        page_table[i].frame = victim_frame;
                        page_table[i].valid = true;
                        page_table[i].dirty = false;
                        pid_t pid = page_table[i].referenced;
                        page_table[i].referenced = 0;

                        printf("Copy data prom a disk (page=%d) to RAM (frame=%d)\n", i, victim_frame);
                        print_frames();
                        printf("Resume MMU process\n");
                        kill(pid, SIGCONT);
                        // SIGCONT
                    } else {
                        page_table[victim_page].valid = false;
                        page_table[victim_page].frame = -1;

                        free_frames[victim_frame] = i;
                        page_table[i].frame = victim_frame;
                        page_table[i].valid = true;
                        page_table[i].dirty = false;
                        pid_t pid = page_table[i].referenced;
                        page_table[i].referenced = 0;

                        printf("Copy data prom a disk (page=%d) to RAM (frame=%d)\n", i, victim_frame);
                        strcpy(RAM[victim_frame], disk[i]);
                        print_frames();

                        disk_access_count++;
                        printf("disk accesses is %d so far\n", disk_access_count);
                        printf("Resume MMU process\n");
                        kill(pid, SIGCONT);
                        // SIGCONT
                    }
                }

            }
            return;
        }
    }
    printf("%d disk access in total\n", disk_access_count);
    printf("Pager is terminated\n");
    munmap(page_table, num_pages * sizeof(struct PTE));
    unlink("/tmp/ex2/pagetable");
    kill(getpid(), SIGTERM);
}


int main(int argc, char *argv[]) {
    mkdir("/tmp/ex2", 0777);
    FILE* frf = fopen("/tmp/ex2/pagetable", "w");
    fclose(frf);

    int pagetable = open("/tmp/ex2/pagetable", O_RDWR);
    ftruncate(pagetable, num_pages * sizeof (struct PTE));
    page_table = (struct PTE *)mmap(NULL, sysconf(_SC_PAGESIZE), PROT_READ | PROT_WRITE,
                                    MAP_SHARED, pagetable, 0);
    signal(SIGUSR1, handler);
    num_pages = atoi(argv[1]);
    num_frames = atoi(argv[2]);
    strcpy(algorithm, argv[3]);
    if (num_frames > MAX_FRAMES || num_pages > MAX_PAGES || num_frames < 1 || num_pages < 1) {
        printf("Error: Invalid input. Number of frames or pages exceeds limits.\n");
        return 1;
    }
    if(!strcmp(algorithm, "random")) {
        printf("Using random page replacement algorithm\n");

    } else if (!strcmp(algorithm, "nfu")) {
        printf("Using nfu page replacement algorithm\n");

    } else if (!strcmp(algorithm, "aging")) {
        printf("Using aging page replacement algorithm\n");
    } else {
        printf("Error: Invalid command\n");
        return 1;
    }
    RAM = (char**)malloc(num_frames * sizeof(char* ));
    disk = (char**)malloc(num_pages * sizeof(char* ));
    for (int i = 0; i < num_frames; i++) {
        RAM[i] = (char*)malloc(MAX_STR * sizeof(char ));
    }
    for (int i = 0; i < num_pages; i++) {
        disk[i] = (char*)malloc(MAX_STR * sizeof(char ));
        for (int j = 0; j < MAX_STR - 1; j++) {
            char x = (char) (rand() %123 + 33);
            if(isprint(x)) disk[i][j] = x;
            else j--;
        }
    }
    printf("Initialised RAM\n");
    print_frames();
    print_disk();
    while (1) {
    }


    return 0;
}

