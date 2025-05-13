#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define FOR(i, a, b) for (int i = (a); i < (b); ++i)

int in, out, buf[5];
sem_t *empty, *full, *mutex;

void *pro(void *arg) {
	int val;
	FOR (i, 0, 5) {
		val = rand() % 100;
		sem_wait(empty);
		sem_wait(mutex);

		buf[in] = val;
		in = (in + 1) % 5;

		sem_post(mutex);
		sem_post(full);
	}
	return NULL;
}

void *cus(void *arg) {
	int val;
	FOR (i, 0, 5) {
		sem_wait(full);
		sem_wait(mutex);

		val = buf[out];
		out = (out + 1) % 5;

		sem_post(mutex);
		sem_post(empty);
	}
	return NULL;
}

int main() {
	pthread_t p[2], c[2];
	int p_id[2], c_id[2];


	empty = sem_open("/empty", O_CREAT, 0644, 5);
	full = sem_open("/full", O_CREAT, 0644, 0);
	mutex = sem_open("/mutex", O_CREAT, 0644, 1);

	FOR (i, 0, 2) {
		p_id[i] = i + 1;
		pthread_create(&p[i], NULL, pro, &p_id[i]);
	}
	FOR (i, 0, 2) {
		c_id[i] = i + 1;
		pthread_create(&c[i], NULL, cus, &c_id[i]);
	}

	FOR (i, 0, 2) pthread_join(p[i], NULL);
	FOR (i, 0, 2) pthread_join(c[i], NULL);
	return 0;
}