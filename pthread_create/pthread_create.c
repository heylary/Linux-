#include <pthread.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void* callback(void* arg) {
    printf("创建子线程\n");
    for(int i = 0; i < 5; i++) {
        printf("子线程的tid%ld\n",pthread_self());
    }
    return NULL;
}

int main(void) {
    pthread_t tid;
    int num = 10;
    int ret = pthread_create(&tid,NULL,callback,(void *)&num);
    if(ret != 0) {
            char *errstr = strerror(ret);
            perror(errstr);
    }

    for(int i = 0; i < 5; i++) {
        printf("%d\n",i);
    }
    printf("子线程的tid:%ld,主线程的tid:%ld\n",tid,pthread_self());
    pthread_exit(NULL);

    return 0;
}