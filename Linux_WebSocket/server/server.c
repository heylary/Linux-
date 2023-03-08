#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>


//服务端通信的流程
// 
int main(void) {
    //1. 创建socket套接字，用于监听客户端数据
    int lfd = socket(AF_INET,SOCK_STREAM,0);
    if(lfd == -1) {
        perror("socket");
        exit(-1);
    }
    //2. 绑定本地ip和port
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = INADDR_ANY;
    saddr.sin_port = htons(9999);
    int ret = bind(lfd,(struct sockaddr*)&saddr,sizeof(saddr));
    if(ret == -1) {
        perror("bind");
        exit(-1);
    }
    //3. 监听
    listen(lfd,8);
    if(ret == -1) {
        perror("listen");
        exit(-1);
    }
    //4. 接收客户端连接
    struct sockaddr_in clientaddr;  //客户端的sockct地址
    int len = sizeof(clientaddr);
    int cfd = accept(lfd,(struct sockaddr*)&clientaddr,&len);
    if(cfd == -1) {
        perror("accept");
        exit(-1);
    }

    //5. 输出客户端数据
    //地址转换
    char clientIP[16];
    inet_ntop(AF_INET,&clientaddr.sin_addr.s_addr,clientIP,sizeof(clientIP));
    unsigned short clientPort = ntohs(clientaddr.sin_port);
    printf("client ip is %s, port is %d",clientIP,clientPort);

    //6.通信
    char recvBuf[1024] = {0};
    while (1)
    {
        memset(recvBuf,0,sizeof(recvBuf));
        /* code */
        //读取客户端发送的数据
        int num = read(cfd,recvBuf,sizeof(recvBuf));
        if(num == -1) {
            perror("read");
            exit(-1);
        }else if(num > 0) {
            printf("recv data is : %s \n",recvBuf);
            sleep(1);
        }else if(num == 0) {
            printf("client closed...");
            break;
        }

        char *data = recvBuf;
        //向客户端发送数据
        int len = write(cfd,data,sizeof(data));
        if(len == -1) {
            perror("write");
            break;
        }else{
            printf("send message to client: %s\n",data);
        }
    }
    close(lfd);
    close(cfd);
    
    return 0;
}