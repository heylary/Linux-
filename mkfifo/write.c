#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

//向管道中写数据

// 总结：
//     读管道：
//         管道中有数据，read返回实际读到的字节数。
//         管道中无数据：
//             写端被全部关闭，read返回0（相当于读到文件的末尾）
//             写端没有完全关闭，read阻塞等待

//     写管道：
//         管道读端全部被关闭，进程异常终止（进程收到SIGPIPE信号）
//         管道读端没有全部关闭：
//             管道已满，write阻塞
//             管道没有满，write将数据写入，并返回实际写入的字节数
int main(void) {
        // 判断文件是否存在
    int ret = access("fifo1", F_OK);
    if(ret == -1) {
        printf("管道不存在，创建管道");

        ret = mkfifo("fifo1",0664);

        if(ret == -1) {
            perror("mkfifo");
            exit(0);
        }
    }

    // 1. 打开管道文件
    int fd = open("fifo1",O_WRONLY);
    if(fd == -1 ) {
        perror("write");
        exit(0);
    }
    // 2. 写入数据
    for(int i = 0; i < 100; i++) {
        char buf[1024];
        sprintf(buf,"hellp,%d\n",i);
        printf("write data: %s\n",buf);
        write(fd,buf,strlen(buf));
        sleep(1);
    }
    close(fd);
    return 0;
}