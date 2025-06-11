#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int calcx(int x) {
	if (x == 1) return 1;
	return calcx(x - 1) * x;
}

int calcy(int y) {
	if (y == 1 || y == 2) return 1;
	return calcy(y - 1) + calcy(y - 2);
}

int main() {
	int x, y, ansx, ansy, ans;
	scanf("%d%d", &x, &y);

	for (int i = 0; i < 2; ++i) {
		int pid, pipefs[2], pipesf[2];
		if (pipe(pipefs) < 0) {
			perror("failed to create pipefs");
			exit(EXIT_FAILURE);
		}
		if (pipe(pipesf) < 0) {
			perror("failed to create pipesf");
			exit(EXIT_FAILURE);
		}
		pid = fork();
		if (pid < 0) {
			perror("failed to create new process");
			exit(EXIT_FAILURE);
		} else if (pid == 0) {
			close(pipefs[0]);
			close(pipesf[1]);

			if (i == 0) {
				read(pipesf[0], &x, sizeof(int));
				int nx = calcx(x);
				write(pipefs[1], &nx, sizeof(int));
			} else {
				read(pipesf[0], &y, sizeof(int));
				int ny = calcy(y);
				write(pipefs[1], &ny, sizeof(int));
			}

			close(pipefs[1]);
			close(pipesf[0]);
			exit(0);
		} else {
			close(pipefs[1]);
			close(pipesf[0]);

			if (i == 0) {
				write(pipesf[1], &x, sizeof(int));
				read(pipefs[0], &ansx, sizeof(int));
				printf("f(x) = %d\n", ansx);
			} else {
				write(pipesf[1], &y, sizeof(int));
				read(pipefs[0], &ansy, sizeof(int));
				printf("f(y) = %d\n", ansy);
			}

			close(pipefs[0]);
			close(pipesf[1]);
			waitpid(pid, NULL, 0);
		}
	}

	printf("f(x, y) = %d\n", ansx + ansy);
	return 0;
}