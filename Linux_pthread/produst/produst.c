    // 创建5个生产者线程，和5个消费者线程
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

pthread_mutex_t mutex;

typedef struct Node
{
    int Num;
    struct Node* next;
}node;

node* head = NULL;

void * produce(void * arg) {
    while(1) {
        pthread_mutex_lock(&mutex);
        node* newnode = (node*) malloc(sizeof(node));
        newnode -> next = head;
        head = newnode;
        newnode -> Num = rand() % 1000;
        printf("ptid: %ld, produce num: %d\n",pthread_self(),newnode->Num);
        pthread_mutex_unlock(&mutex);
        usleep(100);
    }
    
    return NULL;
}

void *customer(void * arg) {
    while(1) {
        pthread_mutex_lock(&mutex);
        node* tmp = head -> next;
        if(head != NULL) {
            head = head -> next;
            printf("ptid: %ld, customer num: %d\n",pthread_self(),tmp->Num);
            free(tmp);
            pthread_mutex_unlock(&mutex);
            usleep(100);
        }else{
            pthread_mutex_unlock(&mutex);
        }
    }
    
    return NULL;
}

int main(void) {
    pthread_mutex_init(&mutex,NULL);
    pthread_t ptid[5],ctid[5]; 

    for(int i = 0; i < 5; i++) {
        pthread_create(&ptid[i],NULL,produce,NULL);
        pthread_create(&ctid[i],NULL,customer,NULL);
    }

    for(int i = 0; i < 5; i++) {
        pthread_detach(ptid[i]);
        pthread_detach(ctid[i]);
    }

    while(1) {
        sleep(10);
    }

    pthread_mutex_destroy(&mutex);
    pthread_exit(NULL);

    

    return 0;
}