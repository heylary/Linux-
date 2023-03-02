#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

/*
    任务:三个子线程同时卖100张票
*/

int ticket = 100;
void * sellTicket(void *arg) {
    while(ticket > 0) {
        printf("%ld正在出售第%d张票\n",pthread_self(),ticket);
        ticket--;
        usleep(6000);
    }
    
    return NULL;
}

int main(void) {

    //创建3个子线程
    pthread_t tid1,tid2,tid3;

    pthread_create(&tid1,NULL,sellTicket,NULL);
    pthread_create(&tid2,NULL,sellTicket,NULL);
    pthread_create(&tid3,NULL,sellTicket,NULL);

    //设置线程分离，结束后自动回收资源
    pthread_detach(tid1);
    pthread_detach(tid2);
    pthread_detach(tid3);

    //终止主线程
    pthread_exit(NULL);

    return 0;
}