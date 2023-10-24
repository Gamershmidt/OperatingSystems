#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MEMORY_SIZE 10000000
unsigned int memory[MEMORY_SIZE];
void allocate_BestFit(unsigned int adrs, int size){
    int bestFitIndex = -1;
    int bestFitSize = MEMORY_SIZE + 1;

    for (int i = 0; i <= MEMORY_SIZE - size; i++) {
        int freeBlockSize = 0;
        for (int j = 0; j < size; j++) {
            if (memory[i + j] == 0) {
                freeBlockSize++;
            } else {
                break;
            }
        }
        if (freeBlockSize == size) {
            bestFitIndex = i;
            break;
        } else if (freeBlockSize > size && freeBlockSize < bestFitSize) {
            bestFitIndex = i;
            bestFitSize = freeBlockSize;
        }
    }

    if (bestFitIndex != -1) {
        for (int j = 0; j < size; j++) {
            memory[bestFitIndex + j] = adrs;
        }
    } else {
        printf("Unable to allocate %d cells for adrs %u using Best Fit.\n", size, adrs);
    }
}
void allocate_FirstFit(unsigned int adrs, int size){
    for (int i = 0; i <= MEMORY_SIZE - size; i++) {
        int found = 1;
        for (int j = 0; j < size; j++) {
            if (memory[i + j] != 0) {
                found = 0;
                break;
            }
        }
        if (found) {
            for (int j = 0; j < size; j++) {
                memory[i + j] = adrs;
            }
            return;
        }
    }
    printf("Unable to allocate %d cells for adrs %u using First Fit.\n", size, adrs);
}
void allocate_WorstFit(unsigned int adrs, int size){
    int worstFitIndex = -1;
    int worstFitSize = 0;

    for (int i = 0; i <= MEMORY_SIZE - size; i++) {
        int freeBlockSize = 0;
        for (int j = 0; j < size; j++) {
            if (memory[i + j] == 0) {
                freeBlockSize++;
            } else {
                break;
            }
        }
        if (freeBlockSize == size) {
            worstFitIndex = i;
            break;
        } else if (freeBlockSize > size && freeBlockSize > worstFitSize) {
            worstFitIndex = i;
            worstFitSize = freeBlockSize;
        }
    }

    if (worstFitIndex != -1) {
        for (int j = 0; j < size; j++) {
            memory[worstFitIndex + j] = adrs;
        }
    } else {
        printf("Unable to allocate %d cells for adrs %u using Worst Fit.\n", size, adrs);
    }
}
void clear(unsigned int adrs) {
    for (int i = 0; i < MEMORY_SIZE; i++) {
        if (memory[i] == adrs) {
            memory[i] = 0;
        }
    }
}
int main() {
    for (int i = 0; i < MEMORY_SIZE; i++) {
        memory[i] = 0;
    }

    FILE *inputFile = fopen("queries.txt", "r");
    if (inputFile == NULL) {
        fprintf(stderr, "Unable to open the input file.\n");
        return 1;
    }

    clock_t start = clock();
    int totalQueries = 0;
    char line[100];

    while (fgets(line, sizeof(line), inputFile)) {
        if (strcmp(line, "end\n") == 0) {
            break; // End of file
        }

        char command[20];
        unsigned int adrs;
        int size;

        if (sscanf(line, "%s %u %d", command, &adrs, &size) == 3) {
            if (strcmp(command, "allocate") == 0) {
                allocate_FirstFit(adrs, size);
            } else if (strcmp(command, "clear") == 0) {
                clear(adrs);
            } else {
                printf("Invalid command: %s\n", command);
            }
            totalQueries++;
        }
    }

    clock_t end = clock();
    double throughput = (double)totalQueries / ((double)(end - start) / CLOCKS_PER_SEC);
    printf("First fit throughput: %f queries/second\n", throughput);

    fclose(inputFile);
    for (int i = 0; i < MEMORY_SIZE; i++) {
        memory[i] = 0;
    }

    inputFile = fopen("queries.txt", "r");
    if (inputFile == NULL) {
        fprintf(stderr, "Unable to open the input file.\n");
        return 1;
    }

    start = clock();
    totalQueries = 0;

    while (fgets(line, sizeof(line), inputFile)) {
        if (strcmp(line, "end\n") == 0) {
            break; // End of file
        }

        char command[20];
        unsigned int adrs;
        int size;

        if (sscanf(line, "%s %u %d", command, &adrs, &size) == 3) {
            if (strcmp(command, "allocate") == 0) {
                allocate_BestFit(adrs, size);
            } else if (strcmp(command, "clear") == 0) {
                clear(adrs);
            } else {
                printf("Invalid command: %s\n", command);
            }
            totalQueries++;
        }
    }

    end = clock();
    throughput = (double)totalQueries / ((double)(end - start) / CLOCKS_PER_SEC);
    printf("Best fit throughput: %f queries/second\n", throughput);

    fclose(inputFile);
    for (int i = 0; i < MEMORY_SIZE; i++) {
        memory[i] = 0;
    }
    inputFile = fopen("queries.txt", "r");
    if (inputFile == NULL) {
        fprintf(stderr, "Unable to open the input file.\n");
        return 1;
    }

    start = clock();
    totalQueries = 0;

    while (fgets(line, sizeof(line), inputFile)) {
        if (strcmp(line, "end\n") == 0) {
            break; // End of file
        }

        char command[20];
        unsigned int adrs;
        int size;

        if (sscanf(line, "%s %u %d", command, &adrs, &size) == 3) {
            if (strcmp(command, "allocate") == 0) {
                allocate_WorstFit(adrs, size);
            } else if (strcmp(command, "clear") == 0) {
                clear(adrs);
            } else {
                printf("Invalid command: %s\n", command);
            }
            totalQueries++;
        }
    }

    end = clock();
    throughput = (double)totalQueries / ((double)(end - start) / CLOCKS_PER_SEC);
    printf("Worst fit throughput: %f queries/second\n", throughput);

    fclose(inputFile);
    for (int i = 0; i < MEMORY_SIZE; i++) {
        memory[i] = 0;
    }
    return 0;
}
