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

        int fd = open(FILE_FROM, O_RDWR|O_TRUNC|O_CREAT);

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

bool copy_file (){
        int fd_read = open(FILE_FROM, O_RDONLY);
        if (fd_read < 0){
                printf ("copy_file can't open file_from\n");
                return false;
        }
        int fd_write = open(FILE_TO, O_CREAT|O_TRUNC|O_WRONLY);
        if (fd_write < 0){
                printf ("copy_file can't open file_to\n");
                return false;
        }

        char *buf = (char*)calloc(BUF_SIZE, sizeof(char));

        int i = 0;
        while (read(fd_read, buf, BUF_SIZE)){
                write (fd_write, buf, BUF_SIZE);
                i++;
                if (i%1000 == 0)
                        printf ("-");
        }
        printf ("\nFile was copied\n");
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
                        sleep (1);
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
