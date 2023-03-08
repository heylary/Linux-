#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <string.h>

int main(void) {
    //1. 创建socket套接字
    int fd = socket(AF_INET,SOCK_STREAM,0);
    if(fd == -1) {
        perror("socket");
        exit(-1);
    }

    //2. 与服务器进行连接
    struct sockaddr_in serveraddr;
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_port = htons(9999);
    inet_pton(AF_INET,"127.0.0.1",&serveraddr.sin_addr.s_addr);
    int ret = connect(fd,(struct sockaddr*)&serveraddr,sizeof(serveraddr));
    if(ret == -1) {
        perror("connect");
        exit(-1);
    }

char recvBuf[1024] = {0};
    //3.通信
    while(1) {
        char *buf = "hello i am client";
        int ret = write(fd,buf,sizeof(buf));
        if(ret == -1) {
            perror("write");
            exit(-1);
        }


        int num = read(fd,recvBuf,sizeof(recvBuf));
        if(num == -1) {
            perror("read");
            exit(-1);
        }else if(num > 0) {
            printf("recp data is : %s",buf);
        }else if(num == 0) {
            printf("server closed...");
            break;
        }

    }
    close(fd);
    return 0;
}