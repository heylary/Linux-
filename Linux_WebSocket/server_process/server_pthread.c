#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

struct sockInfo
{
    int fd;
    struct sockaddr_in addr;
    pthread_t tid;
};

struct sockInfo sockinfos[128];

void * working(void * arg) {
    struct sockInfo* pinfo  = (struct sockInfo*) arg;
    //5.客户端信息
    char buf[16];
    inet_ntop(PF_INET,&pinfo->addr.sin_addr.s_addr,buf,sizeof(buf));
    unsigned short cliPort = ntohs(pinfo->addr.sin_port);
    printf("client ip: %s, port is %d\n",buf,cliPort);
    //6.通信
    char recvBuf[1024];
    while(1) {
        memset(recvBuf,0,sizeof(recvBuf));
        int num = read(pinfo->fd,recvBuf,sizeof(recvBuf));
        if(num == -1) {
            perror("read");
            exit(-1); 
        }else if(num > 0){
            printf("server read data:%s\n",recvBuf);
        }else if(num == 0) {
            printf("client closed\n");
            break;
        }

        write(pinfo->fd,recvBuf,strlen(recvBuf) + 1);
    }
    close(pinfo->fd);
    return NULL;
}


int main() {
     //调用socket套接字监听
    int lfd = socket(PF_INET,SOCK_STREAM,0);
    if(lfd == -1) {
        perror("socket");
        exit(-1);
    }

    //2.bing绑定ip和端口号
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = ntohs(9999);
    saddr.sin_addr.s_addr = INADDR_ANY;

    int ret = bind(lfd,(struct sockaddr*)&saddr,sizeof(saddr));
    if(ret == -1) {
        perror("bind");
        exit(-1);
    }

    //3. 监听
    ret = listen(lfd,128);
    if(ret == -1) {
        perror("listen");
        exit(-1);
    }

    //初始化数据
        int max = sizeof(sockinfos) / sizeof(sockinfos[0]);
        for(int i = 0; i < max; i++) {
            bzero(&sockinfos[i],sizeof(sockinfos[i]));
            sockinfos->fd = -1;
            sockinfos->tid = -1;
        }
 
    while(1) {
        struct sockaddr_in cliaddr;
        int len = sizeof(cliaddr);
        int cfd = accept(lfd, (struct sockaddr*)&cliaddr, &len);
        struct sockInfo *pinfo;
        for(int i = 0; i < max; i++) {
            if(sockinfos[i].fd == -1) {
                pinfo = &sockinfos[i];
            }
            if(i == max - 1) {
                sleep(1);
                i--;
            }
        }
        pinfo->fd = cfd;
        memcpy(&pinfo->addr,&cliaddr,len);
   //  创建子线程
        pthread_create(&pinfo->tid,NULL,working,pinfo);
        pthread_detach(pinfo->tid);
    }
    close(lfd);

    return 0;
}