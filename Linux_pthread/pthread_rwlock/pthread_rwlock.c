
/*
创建8个线程，3个线程写，八个线程读
*/
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

int Num = 1;
// pthread_mutex_t mutex;
pthread_rwlock_t rwlock;

void *writeNum(void * arg) {    //读取数字
    while(1) {
        // pthread_mutex_lock(&mutex);
        pthread_rwlock_wrlock(&rwlock);
        Num++;
        printf("+++Write线程号:%ld读取的数字是%d\n",pthread_self(),Num);
        // pthread_mutex_unlock(&mutex);
        pthread_rwlock_unlock(&rwlock);
        usleep(100);
    }
    return NULL;
}   

void *readNum(void * arg) {
    while(1) {
        // pthread_mutex_lock(&mutex);
        pthread_rwlock_rdlock(&rwlock);
        printf("===Read线程号:%ld读取的数字是%d\n",pthread_self(),Num);
        // pthread_mutex_unlock(&mutex);
        pthread_rwlock_unlock(&rwlock);
        usleep(100);
    }
    return NULL;
}

int main(void) {
    //互斥量初始化
    // pthread_mutex_init(&mutex,NULL);
    //读写锁初始化
    pthread_rwlock_init(&rwlock,NULL);

    pthread_t wtid[3],rtid[5];  //创建线程号变量
    for(int i = 0; i < 3; i++) {
        pthread_create(&wtid[i],NULL,writeNum,NULL);    //创建线程
    }

    for(int i = 0; i < 5; i++) {
        pthread_create(&rtid[i],NULL,readNum,NULL);
    }

    //创建线程分离
    for(int i = 0; i < 3; i++) {
        pthread_detach(wtid[i]);
    }

    for(int i = 0; i < 5; i++) {
        pthread_detach(rtid[i]);
    }

    //关闭主线程号
    pthread_exit(NULL);

    //互斥量关闭
    // pthread_mutex_destroy(&mutex);
    //读写锁关闭
    pthread_rwlock_destroy(&rwlock);

    return 0;
}