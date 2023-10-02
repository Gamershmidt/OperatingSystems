#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <time.h>

#define VECTOR_SIZE 120


int dotProduct(int u[], int v[], int start, int end) {
    int result = 0;
    for (int i = start; i < end; i++) {
        result += u[i] * v[i];
    }
    return result;
}

int main() {
    int u[VECTOR_SIZE];
    int v[VECTOR_SIZE];
    int n;

    for (int i = 0; i < VECTOR_SIZE; i++) {
        u[i] = rand() % 100;
        v[i] = rand() % 100;
    }

    printf("Enter the number of processes: ");
    scanf("%d", &n);

    if (n <= 0 || n > 10) {
        printf("Invalid number of processes.\n");
        return 1;
    }
    int elementsPerProcess = VECTOR_SIZE / n;

    FILE *file = fopen("temp.txt", "w+");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }
    pid_t pid[n];
    
    for (int i = 0; i < n; i++) {
        pid[i] = fork();


        if (pid[i] == 0) { 
            int start = i * elementsPerProcess;
            int end = (i == n - 1) ? VECTOR_SIZE : start + elementsPerProcess;
            int result = dotProduct(u, v, start, end);

            
            fprintf(file, "%d\n", result);

            fclose(file);
            exit(0);
        }
    }

    
    for (int i = 0; i < n; i++) {
        wait(NULL);
    }

    fseek(file, 0, SEEK_SET);
    int totalResult = 0;
    for (int i = 0; i < n; i++) {
        int result;
        fscanf(file, "%d", &result);
        totalResult += result;
    }

    printf("Dot product: %d\n", totalResult);

    fclose(file);

    return 0;
}

