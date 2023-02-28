#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

void myalarm(int num) { //
    printf("捕捉到了信号的编号是%d\n",num);
    printf("xxxx\n");
}

int main(void) {
    //设置捕捉信号
    // typedef void (*sighandler_t)(int);
    // signal(SIGALRM,SIG_DFL);
    signal(SIGALRM,myalarm);


    // 过三秒后，没两秒定时一次
    struct itimerval new_value;

    //设置值
    new_value.it_interval.tv_sec = 2;
    new_value.it_interval.tv_usec = 0;

    new_value.it_value.tv_sec = 3;
    new_value.it_value.tv_usec = 0;
    

    int ret = setitimer(ITIMER_REAL,&new_value,NULL);
    printf("定时器开始了");
    if(ret == -1) {
        perror("settimmer");
        exit(0);
    }

    getchar();

    return 0;
}