#include <stdio.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>

int main(void) {
    // 1.打开一个文件
    int fd = open("text.txt",O_RDWR);
    // 2.获取内存大小
    int size = lseek(fd,0,SEEK_END);
    // 3.创建内存映射区
    void *ptr = mmap(NULL,size,PROT_READ | PROT_WRITE,MAP_SHARED,fd,0);
    if(ptr == MAP_FAILED) {
        perror("mmap");
        exit(0);
    }

    //4.创建父子进程
    pid_t pid =  fork();
    if(pid > 0) {
        wait(NULL);
        char buf[64];
        strcpy(buf,(char *)ptr);
        printf("read buf: %s\n ",buf);
    }else if(pid == 0) {
        strcpy((char*)ptr, "hello a , father"); 
    }

    munmap(ptr,size);

    return 0;
}