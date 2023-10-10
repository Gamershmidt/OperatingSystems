#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>

bool is_prime(int n) {
    if (n <= 1) return false;
    for (int i = 2; i * i <= n; i++) {
        if (n % i == 0) return false;
    }
    return true;
}

pthread_mutex_t global_lock = PTHREAD_MUTEX_INITIALIZER;

int k = 0;
int c = 0;
int n = 0;

int get_number_to_check() {
    int ret;
    pthread_mutex_lock(&global_lock);
    ret = k;
    if (k != n) k++;
    pthread_mutex_unlock(&global_lock);
    return ret;
}

void increment_primes() {
    pthread_mutex_lock(&global_lock);
    c++;
    pthread_mutex_unlock(&global_lock);
}

void *check_primes(void *arg) {
    while (1) {
        int num_to_check = get_number_to_check();
        if (num_to_check >= n) break;

        if (is_prime(num_to_check)) {
            increment_primes();
        }
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <n> <m>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    n = atoi(argv[1]);
    int m = atoi(argv[2]);

    pthread_t threads[m];

    for (int i = 0; i < m; i++) {
        pthread_create(&threads[i], NULL, check_primes, NULL);
    }

    for (int i = 0; i < m; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Total primes in the range [0, %d) = %d\n", n, c);

    return 0;
}

