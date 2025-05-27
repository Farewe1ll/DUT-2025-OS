#include "ipc.h"
#include <unistd.h>

int sem_tobacco_paper, sem_paper_glue, sem_tobacco_glue, sem_done;

int main() {
	key_t key1 = 1234, key2 = 1235, key3 = 1236, key_done = 1237;
	int sem_flg = IPC_CREAT | 0666;

	sem_tobacco_paper = set_sem(key1, 0, sem_flg);
	sem_paper_glue = set_sem(key2, 0, sem_flg);
	sem_tobacco_glue = set_sem(key3, 0, sem_flg);
	sem_done = set_sem(key_done, 0, sem_flg);

	while (1) {
		static int i = 0;
		switch (i % 3) {
			case 0: up(sem_tobacco_paper); break;
			case 1: up(sem_paper_glue); break;
			case 2: up(sem_tobacco_glue); break;
		}
		i++;
		down(sem_done);
		sleep(1);
	}
	return 0;
}