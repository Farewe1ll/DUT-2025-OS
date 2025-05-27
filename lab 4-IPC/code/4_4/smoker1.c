#include "ipc.h"

int sem_tobacco_paper, sem_done;

int main() {
	key_t key1 = 1234, key_done = 1237;

	sem_tobacco_paper = get_ipc_id("/proc/sysvipc/sem", key1);
	sem_done = get_ipc_id("/proc/sysvipc/sem", key_done);

	for (; ;) {
		down(sem_tobacco_paper);
		printf("Smoker with glue is smoking.\n");
		up(sem_done);
	}
	return 0;
}