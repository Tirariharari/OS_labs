#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <sys/wait.h>

int main()
{
	long int ttime;
	pid_t ff;
	int err=0;
	for (int i=0; i<2; i++)
	{
		switch (ff=fork())
		{
			case -1:			// Err
				ttime = time (NULL);
				printf ("Error at %d iteration %s\n", i, ctime(&ttime));
				break;
			case 0:				// Child
				ttime = time (NULL);
				printf ("Child - %x, my parent pid - %x, at %d iteration %s\n",
					 getpid(), getppid(), i, ctime (&ttime));
				break;
			default:
				ttime = time (NULL);
				printf ("Parent - %x, my parent pid - %x, at %d iteration %s\n",
					 getpid(), getppid(), i, ctime (&ttime));
				waitpid (ff, &err, WUNTRACED);	
			break;
		}
		fflush(stdout);
		
	}
	return 0;
}
