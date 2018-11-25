#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>

char data [] = "0987654321qwerty";

int main ()
{
	int file_from = open ("file_from", O_CREAT|O_RDWR|O_TRUNC);  //, "file_from", 0400+0200+0004);
	if (file_from <=0){
		printf ("Can't open file to read\n");
		return 0;
	}
	int file_to = open ("file_to", O_CREAT|O_WRONLY|O_TRUNC|O_APPEND);  //, "file_in", 0400+0200+0004);
	if (file_to <=0){
		printf ("Can't open file to write\n");
		return 0; 
	}

	//  File_from filling
	for (unsigned long long int i = 0; i < 10000000; i++){
		write (file_from, data, 16*sizeof(char));
	}

	time_t t_start = time (0);
	
	char buff [16];
	int control_bytes = 10;
	for (unsigned long long int i = 0; control_bytes!=0; i++)
	{
		lseek (file_from, 16*i, SEEK_SET);
		control_bytes = read (file_from, buff, 16*sizeof(char));
		//	
		write (file_to, buff, 16*sizeof(char));
	}

	time_t t_end = time (0);
	double time_res = difftime (t_end, t_start);
	printf("%f sec.\n", time_res);
	file_from = close (file_from);
	file_to = close (file_to);
	//  printf ("%d  %d", file_from, file_to);
	return 0;
}
