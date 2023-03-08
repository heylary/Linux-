#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>
int main(void) {
    //1.创建socket套接字
    int lfd = socket(AF_INET,SOCK_STREAM,0);
    if(lfd == -1) {
        perror("socket");
        exit(-1);
    }

    //2. 绑定地址和端口
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr=INADDR_ANY;
    saddr.sin_port = htons(9999);
    int ret = bind(lfd,(struct sockaddr*)&saddr,sizeof(saddr));
    if(ret == -1) {
        perror("bind");
        exit(-1);
    }

    //3.监听
    listen(lfd,8);
    if(ret == -1) {
        perror("listen");
        exit(-1);
    }

    //4. 接收客户端连接
    struct sockaddr_in clientaddr;
    int len = sizeof(clientaddr);
    int cfd = accept(lfd,(struct sockaddr*)&clientaddr,&len);
    if(cfd == -1) {
        perror("accept");
        exit(-1);
    }                                         
    //5. 显示客户端信息
    char buf[16];
    inet_ntop(AF_INET,&clientaddr.sin_addr.s_addr,buf,sizeof(buf));
    unsigned short clientPort= ntohs(clientaddr.sin_port);
    printf("client ip: %s, port %d",buf,clientPort);

    //通信
    char recvBUF[1024] = {0};
    while(1) {
        memset(recvBUF,0,sizeof(recvBUF));
        int num = read(cfd,recvBUF,sizeof(recvBUF));
        if(num == -1) {
            perror("read");
            exit(-1);
        }else if(num > 0) {
            printf("recp data is : %s",buf);
        }else if(num == 0) {
            printf("client closed...");
            break;
        }
        char *buf = "hello i am server";
        int ret = write(cfd,buf,sizeof(buf));
        if(ret == -1) {
            perror("write");
            exit(-1);
        }
    }

    close(cfd);
    close(lfd);

    return 0;
}