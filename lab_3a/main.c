#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

#define FIFO_FILE "not_existing_file_for_fifo"

int main (){
	//  Creating fifo
	unlink (FIFO_FILE);
	int fifo = mkfifo (FIFO_FILE, 0777);
	if (fifo == -1){
		printf ("FIFO creation err\n");
		return 0;
	}

	//  "Forking" and "chatting"
	pid_t pid;
	switch (pid = fork()){
	case -1:  //  Err
		printf ("Fork err\n");
		break;
	case 0:  //  Child
	{
		//sleep (2);
		int fifo_read = open(FIFO_FILE, O_RDONLY);
		if (fifo_read == -1){
			printf ("Cannot open fifo to read\n");
			return 0;
		}
		int len = 0;
		char *buf = (char*) calloc (100, sizeof (char));
		read (fifo_read, buf, 100);
		close (fifo_read);
		printf ("%s\n", buf);
		break;
	}
	default:  //  Parent
	{
		int fifo_write = open(FIFO_FILE, O_WRONLY);
		if (fifo_write == -1){
			printf ("Cannot open fifo to write\n");
			return 0;
		}
		time_t t_p = time (0);
		write (fifo_write, ctime(&t_p), 100);
		close (fifo_write);
		break;
	}
	}
	return 0;
}
