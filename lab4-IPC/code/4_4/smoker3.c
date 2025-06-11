#include "ipc.h"

int sem_tobacco_glue, sem_done;

int main() {
	key_t key3 = 1236, key_done = 1237;

	sem_tobacco_glue = get_ipc_id("/proc/sysvipc/sem", key3);
	sem_done = get_ipc_id("/proc/sysvipc/sem", key_done);

	for (; ;) {
		down(sem_tobacco_glue);
		printf("Smoker with paper is smoking.\n");
		up(sem_done);
	}
	return 0;
}