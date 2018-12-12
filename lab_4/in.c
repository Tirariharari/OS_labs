#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <time.h>
#include <sys/shm.h>
#include <sys/types.h>

#define FILE "file_for_shared_memory"

int main(){
	/*
	int file = open (FILE, O_CREAT|O_RDWR|O_TRUNC);
	if (file <= 0){
		printf ("Can't create file\n");
		return 0;
	}

	write (file, "123456789", 9*sizeof(char));	
	*/

	key_t key = ftok(FILE, 29);
	int smid = shmget (2002, 32, IPC_CREAT|0666);
	if (smid == -1)
	{
		printf ("Shared memory creation failed\n");
		return 0;
	}

	char* addr = shmat (smid, NULL, 0);
	if (addr == (char*)-1){
		printf ("Shmat failed\n");
		return 0;
	}

	while (1){
		time_t tt = time(0);
		sprintf(addr, "%s", ctime(&tt));
		sleep(1);
	}

	//file = close (file);
	printf ("%d, %d\n", key, smid);
	return 0;
}
