#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"


char *getFileName(char *path);
void find(char *path, char *pattern);

int main(int argc, char* argv[])
{
    if(argc<3)
    {
        fprintf(2,"usage: find [path] [pattern]\n");
        exit(1);
    }
    char *destFileName=argv[2];
    char *searchDir=argv[1];
    find(searchDir,destFileName);
    exit(0);
}

void find(char *path, char *pattern)
{
    if (strcmp(path,"")==0)
        return;
    
    char buf[512],*p;
    int fd;
    struct stat st;
    struct dirent dirItem;
    
    if((fd=open(path,0))<0)
    {
        fprintf(2,"path: %s is invalid!\n",path);
        return;
    }
    if(fstat(fd,&st)<0)
    {
       fprintf(2,"cannot stat %s!",path);
       close(fd);
       return; 
    }
    switch (st.type)
    {
    case T_DIR:
        
        if(strlen(path)+1+DIRSIZ+1>sizeof buf)
            fprintf(2,"path is too long!\n");
        else
        {
            //遍历目录项
            strcpy(buf,path);
            p=buf+strlen(buf);
            *p++='/';
            while (read(fd,&dirItem,sizeof dirItem)==sizeof(dirItem))
            {
                if(dirItem.inum==0)
                    continue;
                if(strcmp(dirItem.name,".")==0 || strcmp(dirItem.name,"..")==0)
                    continue;
                //
                memmove(p,dirItem.name,DIRSIZ);
                p[DIRSIZ]='\0';
                find(buf,pattern);
            }
        }
        break;
    case T_FILE:
        //从path获取文件名
        if(strcmp(getFileName(path),pattern)==0)
            printf("%s\n",getFileName(path));
    default:
        break;
    }
    close(fd);

}
//从path获取文件名
char *getFileName(char *path)
{
    //不能返回局部变量
    static char buf[DIRSIZ+1];
    
    //查找最右边的“/”
    int i;
    for (i = strlen(path)-1; i >= 0 && path[i]!='/'; i--);
    
    int fileNameLen=strlen(path+i);
    if(fileNameLen>DIRSIZ)
    {
        fprintf(2,"fileName is too long!\n");
        return "";
    }
    memmove(buf,path+i,fileNameLen);
    memset(buf + fileNameLen, ' ', DIRSIZ - fileNameLen);
    if (strlen(buf) == 0) return buf;
    for (int j = strlen(buf) - 1; j > 0; j--) {
        if (buf[j - 1] != ' ') {
            buf[j] = '\0';
            return buf;
        }
    }
    return buf;
}