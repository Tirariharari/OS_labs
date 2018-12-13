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
        /*
        int file = open (FILE, O_CREAT|O_RDWR|O_TRUNC);
        if (file <= 0){
                printf ("Can't create file\n");
                return 0;
        }

        write (file, "123456789", 9*sizeof(char));
        */

        //key_t key = ftok(FILE, 29);
        
	//int sem = semget (2003, 1, 0666);

	int smid = shmget (2002, 32, 0666);

	int sem = semget (2002, 1, 0666);
	if (sem == -1){
		printf ("Err in creation semaphore\n");
		return 0;
	}
	
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
                semop (sem , &sem_lock, 1);
                sleep (1);
		printf("%s", addr);
		semop (sem, &sem_open, 1);
        }

        //file = close (file);
        //printf ("%d, %d\n", key, smid);
        return 0;
}
