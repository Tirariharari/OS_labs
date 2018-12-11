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
	int fifo = mkfifo (FIFO_FILE, 0);

	//  "Forking" and "chatting"
	pid_t pid;
	switch (pid = fork()){
	case -1:  //  Err
		printf ("Fork err\n");
		break;
	case 0:  //  Child
	{
		int fifo_read = open(FIFO_FILE, O_RDONLY);
		int len = 0;
		time_t t_c;
		char *buf = (char*) calloc (100, sizeof (char));
		read (fifo_read, buf, 100);
		close (fifo_read);
		printf ("%s\n", ctime(&t_c));
		break;
	}
	default:  //  Parent
	{
		int fifo_write = open(FIFO_FILE, O_WRONLY);
		time_t t_p = time (0);
		write (fifo_write, ctime(&t_p), sizeof (ctime(&t_p)));
		close (fifo_write);
		break;
	}
	}
	return 0;
}
