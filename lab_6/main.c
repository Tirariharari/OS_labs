#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <time.h>
#include <sys/types.h>
#include <pthread.h>
#include <sys/shm.h>

#define FILE "/tmp"

pthread_mutex_t mutex;
pthread_cond_t condition;

void* thread_func (){
        key_t key = ftok ("FILE", 123);
	int smid = shmget (key, 64, 0666);

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
                pthread_cond_signal (&condition);
                pthread_mutex_unlock(&mutex);
                sleep (3);
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

        key_t key = ftok("FILE", 123);
        int smid = shmget (key, 64, IPC_CREAT|0666);

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
                pthread_cond_wait(&condition, &mutex);
                printf ("%s\n", addr);
                pthread_mutex_unlock(&mutex);
                sleep(1);
        }

        pthread_mutex_destroy(&mutex);
        return 0;
}
