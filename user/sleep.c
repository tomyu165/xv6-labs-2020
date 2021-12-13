#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"


int main(int argc, char* argv[])
{
    if(argc !=2)
    {
        fprintf(2,"usage: grep sleep [duration]\n");
        exit(1);
    }
    
    int duration=atoi(argv[1]);
    sleep(duration);
    exit(0);
}
