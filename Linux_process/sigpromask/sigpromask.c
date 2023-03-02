#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>

// 编写一个程序，把所有的常规信号（1-31）的未决状态打印到屏幕
// 设置某些信号是阻塞的，通过键盘产生这些信号

int main(void) {
    //设置2、3信号阻塞
    
    sigset_t set;
    sigemptyset(&set);

    sigaddset(&set,SIGINT);
    sigaddset(&set,SIGKILL);

    sigprocmask(SIG_BLOCK,&set,NULL);

    while(1) {
        sigset_t pendingset;
        sigemptyset(&pendingset);
        sigpending(&pendingset);

        for(int i = 1; i < 32; i++) {
            if(sigismember(&pendingset,i) == 1) {
                printf("1");
            }else if(sigismember(&pendingset,i) == 0) {
                printf("0");
            }else {
                perror("sigismember");
                exit(0);
            }
        }
        printf("\n");
        sleep(1);
        
    }
    return 0;
}