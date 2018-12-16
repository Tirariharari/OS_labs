#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <time.h>
#include <sys/types.h>
#include <pthread.h>
#include <sys/shm.h>

#define FILE "file_for_shared_memory"

pthread_mutex_t mutex;

void* thread_func (){
	int smid = shmget (2002, 32, 0666);
	
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

	time_t timer;// = time(0);
	while (1){
		timer = time(0);
        	pthread_mutex_lock(&mutex);
       		sprintf(addr, "%s", ctime(&timer));
        	pthread_mutex_unlock(&mutex);
		sleep (1);
	}
	return 0;
}

int main(){
	//  Thread initialization
	pthread_t pth;

	pthread_mutex_init (&mutex, NULL);
	int stat = pthread_create (&pth, NULL, thread_func, NULL);
	if (stat != 0){
		printf ("%d thread create fialed\n", stat);
		return 0;
	}

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
		pthread_mutex_lock (&mutex);
		printf ("%s\n", addr);
		pthread_mutex_unlock(&mutex);
		sleep(1);
	}

	pthread_mutex_destroy(&mutex);
	return 0;
}

