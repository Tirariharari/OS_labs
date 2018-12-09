#include <stdlib.h>
#include <dirent.h> 
#include <unistd.h>
#include <stdio.h> 
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>

int main(void)
{
	DIR *d;
	int n = 0;
	struct dirent *dir = (struct dirent*) calloc (sizeof (struct dirent)+1, 1);
	struct dirent **namelist;
	struct stat *buff;
	time_t t;
	buff = (struct stat*) calloc (sizeof(struct stat), 1);
	n = scandir (".", &namelist, NULL, alphasort);
	int i=0;
	while (i<n)
	{
		dir = namelist[i];
		stat (dir->d_name, buff);
		
		printf ((buff->st_mode & S_IFDIR)?"d":"-");
		printf ((buff->st_mode & S_IRUSR)?"r":"-");
		printf ((buff->st_mode & S_IWUSR)?"w":"-");
		printf ((buff->st_mode & S_IXUSR)?"x":"-");
		printf ((buff->st_mode & S_IRGRP)?"r":"-");
		printf ((buff->st_mode & S_IWGRP)?"w":"-");
		printf ((buff->st_mode & S_IXGRP)?"x":"-");
		printf ((buff->st_mode & S_IROTH)?"r":"-");
		printf ((buff->st_mode & S_IWOTH)?"w":"-");
		printf ((buff->st_mode & S_IXOTH)?"x":"-");
		
		char mtime[80];
		t = buff->st_mtime;
		struct tm lt;
		localtime_r(&t, &lt);
		strftime(mtime, sizeof mtime, "%d %b %Y %T", &lt);
		printf ("\t%s", mtime);
		printf ("\t%5lld\t %s\n", buff->st_size, dir->d_name);
		free (namelist[n]);
		i++;
	}
	return(0);
}
