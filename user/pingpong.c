#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

#define READFD 0
#define WRITEFD 1

int main(int argc, char* argv[])
{

    int p2c[2];//管道是一小块内核缓冲区，分读端和写端
    int c2p[2];
    
    pipe(c2p);//创建管道，单向，返回的是一个文件描述符，每个进程都有独立的文件描述符，指向系统文件描述符
    pipe(p2c);
    
    char buf[64];
    int pid=fork();
    if(pid>0) //父进程
    {
        close(c2p[WRITEFD]);
        close(p2c[READFD]);

        //读ping
        read(c2p[READFD],buf,5);
        close(c2p[READFD]);
        printf("%d: received %s\n",pid,buf);

        //写pong
        write(p2c[WRITEFD],"pong",5);
        close(p2c[WRITEFD]);
        //退出
        wait(&pid);
        exit(0);
    }else if(pid==0) //子进程
    {
        close(c2p[READFD]);
        close(p2c[WRITEFD]);

        write(c2p[WRITEFD],"ping",5);
        close(c2p[WRITEFD]);

        read(p2c[READFD],buf,5);
        close(p2c[READFD]);
        printf("%d: received %s\n",pid,buf);
        exit(0);
    }
    exit(0);
}

