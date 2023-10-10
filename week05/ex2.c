#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#define SIZE 256
struct Thread {
	pthread_t id;
	int i;
	char message[SIZE];
};
void *thr_func(void *arg) {
	struct Thread *data = (struct Thread *)arg;
	printf("Thread %d is created\n", data->i);
        snprintf(data->message, SIZE, "Hello from thread %d", data->i);
        printf("Thread %d prints message: %s\n", data->i, data->message);
        pthread_exit(NULL);
}
int main(int argc, char *argv[]) {
	int n = atoi(argv[1]);
	struct Thread threads[n];
	for(int i = 0; i < n; i++) {
		threads[i].i = i + 1;
		pthread_create(&threads[i].id, NULL, thr_func, &threads[i]);
                pthread_join(threads[i].id, NULL);
	}
	return 0;
}
