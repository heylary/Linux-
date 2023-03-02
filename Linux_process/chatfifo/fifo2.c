#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h> 
#include <sys/types.h>
#include <sys/stat.h>

int main(void) {
    //1. 创建管道
    //判断管道是否存在
    int ret = access("fifo1",F_OK);
    if(ret == -1) {

        printf("管道不存在，创建管道1");
        ret = mkfifo("fifo1",0664);
        
        if(ret == -1) {
            perror("mkfifo");
            exit(0);
        }         
    }

    ret = access("fifo2",F_OK);
    if(ret == -1) {
        printf("管道不存在，创建管道2");
        ret = mkfifo("fifo2",0664);
        
        if(ret == -1) {
            perror("mkfifo2");
            exit(0);
        }         
    }

    
    int fdr = open("fifo1",O_RDONLY);
    if(fdr == -1) {
        perror("open");
        exit(0);
    }
    printf("打开管道fifo1成功，等待读取..");

    int fdw = open("fifo2",O_WRONLY);
    if(fdr == -1) {
        perror("open");
        exit(0);
    }
    printf("打开管道fifo2成功，等待写入数据");

    char buf[128];

    while(1) {

        //读数据
        memset(buf,0,128);
        ret = read(fdr,buf,sizeof(buf));
        if(ret <= 0) {
            perror("read");
            break;
        }
        printf("buf: %s",buf);


        memset(buf,0,128);
        //获取标准输入数据
        fgets(buf,128,stdin);
        //写数据
        ret = write(fdw,buf,sizeof(buf));
        if(ret == -1) {
            perror("write");
            exit(0);
        }
    }

    //关闭文件描述符
    close(fdw);
    close(fdr);

    return 0;
} 