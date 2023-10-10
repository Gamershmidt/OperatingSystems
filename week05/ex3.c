#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

typedef struct prime_request {
    int a, b;
} prime_request;

bool is_prime(int n) {
    if (n <= 1) return false;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) return false;
    }
    return true;
}


void *prime_counter(void *arg) {
    int count = 0;
    prime_request *req = (prime_request *)arg;

    for (int i = req->a; i < req->b; i++) {
        if (is_prime(i)) {
            count++;
        }
    }

    int *result = (int *)malloc(sizeof(int));
    *result = count;
    pthread_exit(result);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <n> <m>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int n = atoi(argv[1]);
    int m = atoi(argv[2]);

    pthread_t threads[m];
    prime_request requests[m];
    int results[m];

    int interval_size = n / m;
    int remaining = n % m;

    for (int i = 0; i < m; i++) {
        requests[i].a = i * interval_size;
        requests[i].b = (i + 1) * interval_size + (i == m - 1 ? remaining : 0);
        pthread_create(&threads[i], NULL, prime_counter, &requests[i]);
    }

    int total_count = 0;
    for (int i = 0; i < m; i++) {
        int *result;
        pthread_join(threads[i], (void **)&result);
        results[i] = *result;
        total_count += results[i];
        free(result);
    }

    printf("Total primes in the range [0, %d) = %d\n", n, total_count);

    return 0;
}

