#include "ipc.h"

int sem_paper_glue, sem_done;

int main() {
	key_t key2 = 1235, key_done = 1237;

	sem_paper_glue = get_ipc_id("/proc/sysvipc/sem", key2);
	sem_done = get_ipc_id("/proc/sysvipc/sem", key_done);

	for (; ;) {
		down(sem_paper_glue);
		printf("Smoker with tobacco is smoking.\n");
		up(sem_done);
	}
	return 0;
}