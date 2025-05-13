#include <stdio.h>
#include <pthread.h>

int i = 42;
void *thread1(void *vargp) {
	printf("%d\n",i);
}

void *thread2(void *vargp) {
	i = 31;
}

int main() {
	pthread_t tid1, tid2;
	pthread_create(&tid2, NULL, thread2, (void*)&i);
	pthread_create(&tid1, NULL, thread1, (void*)&i);
	pthread_join(tid1, (void**)&i);
	pthread_join(tid2, NULL);
}