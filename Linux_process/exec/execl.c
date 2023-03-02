#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
// int execl(const char *pathname, const char *arg, ...
//                        /* (char  *) NULL */);
/*
    -path: 需要指定的执行文件的路径或内容
    -arg: 执行可执行程序的参数
        第一个参数一般写文件名，没什么作用
        从第二个参数开始，就是程序执行所需要的参数列表
        参数最后需要以NULL结束
    返回值：
        只有调用失败才会有返回值，
*/

int main() {
    //创建一个子进程，在子进程中执行exec函数族中的函数
    pid_t pid = fork();
    if(pid > 0) {
        printf("i am parent process pid : %d\n",getpid());
    }else if(pid == 0) {
        execl("hello","hello","NULL");

        printf("i am child process pid = %d\n",getpid());
    }

    for(int i = 0; i < 3; i++) {
        printf("i = %d pid = %d",i,getpid());
    }

    return 0;
}