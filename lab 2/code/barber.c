#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <fcntl.h>

#define FOR(i, a, b) for (int i = (a); i < (b); ++i)
#define N 5

sem_t *customers, *barbers, *mutex;
int waiting = 0;

void *barber_function(void *arg) {
	int barber_id = *((int *)arg);

	for (; ;) {
		sem_wait(customers);
		sem_wait(mutex);

		--waiting;
		sleep(rand() % 3 + 1);

		sem_post(barbers);
		sem_post(mutex);
	}
	return NULL;
}

void *customer_function(void *arg) {
	int customer_id = *((int *)arg);

	sem_wait(mutex);

	if (waiting < N) {
		++waiting;
		sem_post(customers);
		sem_post(mutex);

		sem_wait(barbers);
		sleep(rand() % 2 + 1);
	} else sem_post(mutex);

	return NULL;
}

int main() {
	pthread_t barber_thread;
	pthread_t customer_threads[10];
	int barber_id = 1;
	int customer_ids[10];


	srand(time(NULL));

	customers = sem_open("/customers", O_CREAT, 0644, 0);
	barbers = sem_open("/barbers", O_CREAT, 0644, 0);
	mutex = sem_open("/mutex", O_CREAT, 0644, 1);

	pthread_create(&barber_thread, NULL, barber_function, &barber_id);

	FOR (i, 0, 10) {
		customer_ids[i] = i + 1;
		pthread_create(&customer_threads[i], NULL, customer_function, &customer_ids[i]);
		sleep(rand() % 2 + 1);
	}


	FOR (i, 0, 10) {
		pthread_join(customer_threads[i], NULL);
	}
	// 理应终止理发师进程
	pthread_join(barber_thread, NULL);
	return 0;
}
