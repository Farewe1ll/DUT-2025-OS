#include <stdio.h>
#include <pthread.h>

int sum = 0;
pthread_mutex_t mutex;

void *thread() {
	int i;
	for (i = 0; i < 1000000; i++) {
		pthread_mutex_lock(&mutex);
		++sum;
		pthread_mutex_unlock(&mutex);
	}
	return NULL;
}
int main() {
	pthread_t tid1, tid2;
	pthread_mutex_init(&mutex, NULL);

	pthread_create(&tid1, NULL, thread, NULL);
	pthread_create(&tid2, NULL, thread, NULL);
	pthread_join(tid1, NULL);
	pthread_join(tid2, NULL);
	printf("1000000 + 1000000 = %d\n", sum);
	return 0;
}