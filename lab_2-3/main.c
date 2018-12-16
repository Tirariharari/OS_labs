#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <time.h>
#include <stdbool.h>

#define SIZE 100000000  //  ~100mb
#define BUF_SIZE 1000
#define FILE_FROM "file_from"
#define FILE_TO "file_to"

char data [] = "0987654321qwerty";

bool file_creation (){
        int fd = open(FILE_FROM, O_RDWR|O_TRUNC|O_CREAT);
        if (fd < 0)
                return false;
        close(fd);
        return true;
}

bool writing_to_file (){
        //  Fulling buffer
        char * buf = (char*)calloc(BUF_SIZE, sizeof(char));
        for (int i=0; i<BUF_SIZE; i++)
                buf[i] = '0' + i%10;

        int fd = open(FILE_FROM, O_RDWR|O_TRUNC|O_CREAT|O_NONBLOCK);

        if (fd < 0){
                return false;
        }else{
                //  Writing to file
                for(int i=0; i< SIZE/BUF_SIZE; i++){
                        write(fd, buf, BUF_SIZE);
                        if (i%1000 == 0)  //  Loading
                                printf ("*");
                }
                printf ("\n");
        }
        close(fd);
        return true;
}

int fd_set_blocking (int fd, int blocking){
	int flags = fcntl (fd, F_GETFL, 0);
	if (flags == -1)
		return 0;
	if (blocking)
		flags &= ~O_NONBLOCK;
	else
		flags |= O_NONBLOCK;
	return fcntl(fd, F_SETFL, flags)!=-1;
}

bool copy_file (){
		//  Openning files
        int fd_read = open(FILE_FROM, O_RDONLY|O_NONBLOCK);
        if (fd_read < 0){
                printf ("copy_file can't open file_from\n");
                return false;
        }
        int fd_write = open(FILE_TO, O_CREAT|O_TRUNC|O_RDWR|O_NONBLOCK);
        if (fd_write < 0){
                printf ("copy_file can't open file_to\n");
                return false;
        }

        //  Set non-block
       	fd_set fds_read;
	fd_set fds_write;

	FD_ZERO (&fds_read);
	FD_ZERO (&fds_write);

        fd_set_blocking (fd_read, 0);
        fd_set_blocking (fd_write, 0);
        
        FD_SET (fd_read, &fds_read);
	FD_SET (fd_write, &fds_write);

	//  Copying file
        char *buf = (char*) calloc (BUF_SIZE, sizeof(char));

        struct timeval tv;
        tv.tv_sec = 10;
        tv.tv_usec = 0;
        int largest_sock = fd_read > fd_write ? fd_read : fd_write;
        
        int ret = select (largest_sock+1, &fds_read, &fds_write, NULL, &tv);
        //int i = 0;
        //while (read (fd_read, buf, BUF_SIZE)){
        if (ret == -1){
        		printf ("Select error\n");
        		return false;
        } else if (ret == 0){
        		printf("No data\n");
        } else{
        		if (FD_ISSET (fd_read, &fds_read)){}
        				//printf ("Why do we have data???\n");
        		if (FD_ISSET (fd_write, &fds_write)){
                		write (fd_write, buf, BUF_SIZE);
                }
        }
        printf ("File was copied\n");
        close (fd_write);
	close (fd_read);
	return true;
}

int main (){
        //  File creation
        if (!file_creation()){
                printf ("Error in file creation\n");
                return 0;
        }

        //  Forking
        pid_t pid = fork();
        switch (pid){
                case -1:
                {
                        printf ("Fork error\n");
                        return 0;
                }
                case 0:  //  Child
                {
                        //  Reading from file and writing to an other
                        if (!copy_file ()){
                                printf ("Err in copying file\n");
                                return 0;
                        }
                        break;
                }
                default:  //  Parent
                {
                        //  Writing to file
                        if (!writing_to_file ()){
                                printf ("Writing to file error\n");
                                return 0;
                        }
			printf ("File was created\n");
                        break;
                }
        }
        return 0;
}

