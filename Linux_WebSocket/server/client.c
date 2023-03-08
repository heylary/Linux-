#include <stdio.h>
#include <arpa/inet.h>
#include <unistd.h>
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
    //将主机字节序转为网络字节序
    inet_pton(AF_INET,"127.0.0.1",&serveraddr.sin_addr.s_addr);   //初始化IP
    serveraddr.sin_port = htons(9999);  //将主机端口转为网络字节序
    int ret = connect(fd,(struct sockaddr*)&serveraddr,sizeof(serveraddr));
    if(ret == -1) {
        perror("connect");
        exit(-1);
    }

    //3.通信
    char recvBuf[1024] = {0};
    while(1) {
        memset(recvBuf,0,sizeof(recvBuf));
        // char * data = "hello,i am client";
        char data[1024];
        printf("请发送数据:\n");
        scanf("%s",data);
        //给服务器发送数据
        write(fd, data, strlen(data));

        int len = read(fd,recvBuf,sizeof(recvBuf));
        if(len == -1) {
            perror("read");
            exit(-1);
        } else if(len > 0) {
            printf("recv server data : %s\n", recvBuf);
        } else if(len == 0) {
            // 表示服务器端断开连接
            printf("server closed...");
            break;
        }
    }

    close(fd);

    return 0;
}