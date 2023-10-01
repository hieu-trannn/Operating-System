/**
 * Demonstrate shared memory for interprocess communication and basic process synchronization.
 * Kien @ 4-2023
*/

#include <sys/types.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

#define SHM_NAME	"shared-struct"

struct shared_vars {
	int x, y, z;
	int ready;
};


int main() {
	int shmId = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
	if (shmId < 0) {
		perror("shm_open failed");
		return 1;
	}

	int shmSize = 2 * sizeof(struct shared_vars);
	ftruncate(shmId, shmSize);

	int childPid = fork();
	if (childPid < 0) {
		shm_unlink(SHM_NAME);
		perror("fork() failed");
		return 1;
	}

	struct shared_vars* parentVars = (struct shared_vars*)mmap(NULL, shmSize, PROT_READ | PROT_WRITE, MAP_SHARED, shmId, 0);
	parentVars->ready = 0;
	//de khi khai bao trong process con, bien ready khong nhan gia tri lung tung

	if (childPid == 0) {	// child process
		struct shared_vars* childVars = (struct shared_vars*)mmap(NULL, shmSize, PROT_READ | PROT_WRITE, MAP_SHARED, shmId, 0);

		while (childVars->ready != 1) usleep(1000);

		printf("Child is active\n");
		
		childVars->z = childVars->x + childVars->y;
		childVars->ready = 0;

		printf("Child terminates\n");

	} else {	// parent process
		printf("x = ");
		scanf("%d", &parentVars->x);

		printf("y = ");
		scanf("%d", &parentVars->y);

		parentVars->ready = 1;

		printf("Parent is inactive\n");

		while (parentVars->ready != 0) usleep(1000);

		printf("Parent is active\n");

		printf("z = %d\n", parentVars->z);

		printf("Parent terminates\n");

		shm_unlink(SHM_NAME);
	}

	return 0;
}
