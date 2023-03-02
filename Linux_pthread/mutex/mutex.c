#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

/*
    任务:三个子线程同时卖100张票
*/

//创建一个互斥量
pthread_mutex_t mutex;

int ticket = 1000;
void * sellTicket(void *arg) {
    
    //临界区加锁、解锁


    while(1) {
        pthread_mutex_lock(&mutex);

        if(ticket > 0) {
            usleep(6000);
            printf("%ld正在出售第%d张票\n",pthread_self(),ticket);
            ticket--;
        }else {
            pthread_mutex_unlock(&mutex);
            break;
        }
        pthread_mutex_unlock(&mutex);
    }

    
    return NULL;
}

int main(void) {

    //初始化互斥量
    pthread_mutex_init(&mutex,NULL);

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
    
    //释放互斥量资源
    pthread_mutex_destroy(&mutex);

    return 0;
}