#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include <semaphore.h>

#define N 10000000

#define NUM_THREADS 4

bool is_prime(int n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;

    for (int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) return false;
    }

    return true;
}

sem_t mutex; 
int prime_count = 0; 

void* count_primes(void* args) {
    int thread_id = *((int*)args);
    int lower_bound = thread_id * (N / NUM_THREADS);
    int upper_bound = (thread_id == NUM_THREADS - 1) ? N : (thread_id + 1) * (N / NUM_THREADS);

    int local_count = 0; 

    for (int i = lower_bound; i < upper_bound; ++i) {
        if (is_prime(i)) {
            local_count++;
        }
    }

    sem_wait(&mutex);
    prime_count += local_count;
    sem_post(&mutex);

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    int thread_args[NUM_THREADS];

    
    sem_init(&mutex, 0, 1);

    
    for (int i = 0; i < NUM_THREADS; ++i) {
        thread_args[i] = i;
        pthread_create(&threads[i], NULL, count_primes, &thread_args[i]);
    }

    
    for (int i = 0; i < NUM_THREADS; ++i) {
        pthread_join(threads[i], NULL);
    }

    printf("Total prime numbers in the range [0, %d) = %d\n", N, prime_count);

    
    sem_destroy(&mutex);

    return 0;
}

