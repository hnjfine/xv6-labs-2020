#include "kernel/types.h"
#include "user/user.h"

int main()
{
    char buf = 'p';
    int fd1[2]; //子到父
    int fd2[2]; //父到子
    pipe(fd1);
    pipe(fd2);
    int exit_status = 0;
    int pid = fork();
    if(pid<0)
    {
        fprintf(2, "fork() error!\n");
        close(fd1[0]);
        close(fd1[1]);
        close(fd2[0]);
        close(fd2[1]);
        exit(1);
    }
    else if(pid==0)
    {
        close(fd2[1]);
        close(fd1[0]);
        if(read(fd2[0],&buf,sizeof(char))!=sizeof(char))
        {
            fprintf(2, "child read() error!\n");
            exit_status = 1; //标记出错
        }
        else
        {
            fprintf(1, "%d: received ping\n", getpid());
        }

        if(write(fd1[1],&buf,sizeof(char))!=sizeof(char))
        {
            fprintf(2, "child write() error!\n");
            exit_status = 1;
        }
        close(fd1[1]);
        close(fd2[0]);
        exit(exit_status);
    }
    else{
        close(fd2[0]);
        close(fd1[1]);

        if(write(fd2[1],&buf,sizeof(char))!=sizeof(char))
        {
            fprintf(2, "parent write() error!\n");
            exit_status = 1;
        }
        if(read(fd1[0],&buf,sizeof(char))!=sizeof(char))
        {
            fprintf(2, "parent read() error!\n");
            exit_status = 1; //标记出错
        } else {
            fprintf(1, "%d: received pong\n", getpid());
        }
        close(fd1[0]);
        close(fd2[1]);
        exit(exit_status);
    }
}