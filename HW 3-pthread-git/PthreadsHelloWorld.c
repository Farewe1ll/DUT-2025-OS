#include <pthread.h>
#include <stdio.h>

void *worker(void *arg)
{
	printf("Hello World!\n");
	return NULL;
}

int main()
{
	pthread_t tid;

	if (pthread_create(&tid, NULL, worker, NULL)) {
		printf("Cannot creating thread\n");
		return 1;
	}

	printf("Main waits for thread to finish...\n");

	pthread_join(tid, NULL);
	return 0;
}