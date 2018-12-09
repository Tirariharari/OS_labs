#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>

int main (int argc, char* argv){
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
		int len = 0;
		time_t tim;
		close (pipes[1]);
		while ((len = read(pipes[0], &tim, sizeof (tim))) != 0)
			write (2, &tim, len);
		close (pipes[0]);
		printf ("%s\n", ctime(&tim));
		break;
	}
	default:
	{
		time_t tt = time (0);
		close (pipes[1]);
		write (pipes[0], (void*)tt, sizeof(tt)+1);
		close (pipes[0]);
		break;
	}
	}
	return 0;
}
