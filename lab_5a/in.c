#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <time.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/sem.h>

#define FILE "file_for_shared_memory"

struct sembuf sem_lock = {0,-1,0}, sem_open = {0,1,0};

int main(){
	int smid = shmget (2002, 32, IPC_CREAT|0666);

	if (smid == -1)
	{
		printf ("Shared memory creation failed\n");
		return 0;
	}

	int sem = semget(2002, 1, IPC_CREAT|0666);
	if (sem == -1){
		printf ("Err in creation semaphore\n");
		return 0;
	}

	char* addr = shmat (smid, NULL, 0);
	if (addr == (char*)-1){
		printf ("Shmat failed\n");
		return 0;
	}

	semop (sem, &sem_open, 1);
	while (1){
		semop(sem, &sem_lock, 1);
		time_t tt = time(0);
		sprintf(addr, "%s", ctime(&tt));
		sleep(2);
		semop (sem, &sem_open, 1);
	}

	return 0;
}
