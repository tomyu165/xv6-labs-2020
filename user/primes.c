#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

#define READFD 0
#define WRITEFD 1

//埃氏筛法
int primes_sieve(int fd);

int main(int argc, char* argv[])
{
    if(argc>2)
    {
        fprintf(2,"usage: primes [number]");
        exit(1);
    }
    int n=35;
    if(argc==2)
    {
        int t=atoi(argv[1]);
        if(t<1)
        {
            fprintf(2,"parameter is invalid!");
            exit(1);
        }
        if(t>35)
            n=t;
    }
    int pp[2];
    pipe(pp);

    for(int i=2;i<=n;i++)
        write(pp[WRITEFD],&i,1);

    close(pp[WRITEFD]);
    primes_sieve(pp[READFD]);
    exit(0);
}

int primes_sieve(int fd)
{
    int pp[2];
    if(0!=pipe(pp))
    {
        fprintf(2,"pipe is failed to create!");
        exit(1);
    }
    
    int first=2,count=0;
    int buf;
    while(read(fd,&buf,1) == 1)
    {
        //第一个一定是素数
        if(count==0)
        {
            count++;
            first=buf;
            printf("prime %d\n",buf);
        }else
        {
            if(buf%first!=0)
            {
                write(pp[WRITEFD],&buf,1);
                count++;
            }
        }
    }
    close(fd);
    close(pp[WRITEFD]);

    if(count<=1)
        exit(0);

    int pid=fork();
    if(pid==0)
        primes_sieve(pp[READFD]);

    wait(&pid);
    exit(0);
}
