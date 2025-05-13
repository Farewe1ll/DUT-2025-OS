#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define FOR(i, a, b) for (int i = (a); i < (b); ++i)

int buffer[5];
int in = 0, out = 0;

sem_t *empty, *full, *mutex;

void *pro(void *arg) {
	int item;
	FOR (i, 0, 5) {
		item = rand() % 100;
		sem_wait(empty);
		sem_wait(mutex);

		buffer[in] = item;

		sem_post(mutex);
		sem_post(full);
	}
	return NULL;
}

void *con(void *arg) {
	int item;
	FOR (i, 0, 5) {
		sem_wait(full);
		sem_wait(mutex);

		item = buffer[out];
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
		pthread_create(&c[i], NULL, con, &c_id[i]);
	}

	FOR (i, 0, 2) pthread_join(p[i], NULL);
	FOR (i, 0, 2) pthread_join(c[i], NULL);
	return 0;
}