#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*子进程结束后，将数据发送给父进程*/

int main(void) {

    //创建一个匿名管道
    int fd[2];
    int ret = pipe(fd);

    if(ret == -1) {
        perror("pipe");
        exit(0);
    }
    //创建子进程
    pid_t pid  = fork();
    if(pid > 0) {
        //父进程
        //从管道中读取数据
        close(fd[1]);
        char buf[1024] = {0};

        int len = -1;
        while( read(fd[0],buf,sizeof(buf) - 1) > 0){
            printf("%s",buf);
            memset(buf,0,1024);
        }
        
        


    }else if(pid == 0) {
        // 子进程
        //关闭读端
        close(fd[0]);

        // 文件描述符的重定向 stdout_fileno -> fd[1]
        dup2(fd[1],STDOUT_FILENO);
        // 执行 ps aux
        execlp("ps","ps","aux",NULL);
        perror("execlp");
        exit(0);
    }else{
        perror("fork");
        exit(0);
    }
}