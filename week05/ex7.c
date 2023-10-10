#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int gcd(int a, int b) {
    if (b == 0) return a;
    return gcd(b, a % b);
}

int lcm(int a, int b) {
    return (a * b) / gcd(a, b);
}


typedef struct {
    int* numbers;  
    int num_count;  
    int result;     
} ThreadData;

void* compute_lcm(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    int local_lcm = data->numbers[0];

    for (int i = 1; i < data->num_count; i++) {
        local_lcm = lcm(local_lcm, data->numbers[i]);
    }

    data->result = local_lcm;

    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <input_file> <T>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    char* input_file = argv[1];
    int T = atoi(argv[2]);

    FILE* file = fopen(input_file, "r");
    if (!file) {
        perror("Failed to open input file");
        exit(EXIT_FAILURE);
    }

    int num_count;
    if (fscanf(file, "%d", &num_count) != 1) {
        fprintf(stderr, "Failed to read the number of integers from the input file\n");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    int* numbers = (int*)malloc(num_count * sizeof(int));
    if (!numbers) {
        perror("Failed to allocate memory for integers");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < num_count; i++) {
        if (fscanf(file, "%d", &numbers[i]) != 1) {
            fprintf(stderr, "Failed to read integer %d from the input file\n", i + 1);
            free(numbers);
            fclose(file);
            exit(EXIT_FAILURE);
        }
    }

    fclose(file);

    pthread_t threads[T];
    ThreadData thread_data[T];

    int integers_per_thread = num_count / T;
    int remaining_integers = num_count % T;
    int current_index = 0;

    for (int i = 0; i < T; i++) {
        thread_data[i].numbers = &numbers[current_index];
        thread_data[i].num_count = integers_per_thread + (i < remaining_integers ? 1 : 0);

        pthread_create(&threads[i], NULL, compute_lcm, &thread_data[i]);

        current_index += thread_data[i].num_count;
    }

    for (int i = 0; i < T; i++) {
        pthread_join(threads[i], NULL);
    }

    int final_lcm = thread_data[0].result;
    for (int i = 1; i < T; i++) {
        final_lcm = lcm(final_lcm, thread_data[i].result);
    }

    printf("Least Common Multiple (LCM) of the integers = %d\n", final_lcm);

    free(numbers);

    return 0;
}

