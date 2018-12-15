#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>

int main (){
	//  Creating pipe
	int pipes[2];
	if (-1 == pipe(pipes))
	{
		printf ("Pipe err. I don't know the reason\n");
		return 0;
	}

	//  "Forking"
	pid_t pid;
	switch (pid = fork()){
	case -1:  //  Err
		printf ("Fork err\n");
		break;
	case 0:  //  Child
	{
		sleep (1);
		time_t buf;
		close (pipes[1]);
		read(pipes[0], &buf, sizeof (time_t));
		close (pipes[0]);
		printf ("%s\n", ctime (&buf));
		break;
	}
	default:
	{
		time_t tt = time (0);
		close (pipes[0]);
		write (pipes[1], &tt, sizeof (time_t));
		close (pipes[1]);
		break;
	}
	}
	return 0;
}
