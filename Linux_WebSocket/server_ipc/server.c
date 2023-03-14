#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/un.h>
#include <arpa/inet.h>
#include <string.h>

int main() {
    unlink("server.sock");
    int lfd = socket(AF_LOCAL,SOCK_STREAM,0);
    struct sockaddr_un saddr;
    saddr.sun_family = AF_LOCAL;
    strcpy(saddr.sun_path,"server.sock");
    //2.绑定本地套接字文件
    int ret = bind(lfd,(struct sockaddr*)&saddr,sizeof(saddr));
    if(ret == -1) {
        perror("bind");
        exit(-1);
    }
    //3.监听
    ret = listen(lfd,100);
    if(ret == -1) {
        perror("listen");
        exit(1);
    }
    //等待并请求连接
    struct sockaddr_un cliaddr;
    int len = sizeof(cliaddr);
    int cfd = accept(lfd,(struct sockaddr*)&cliaddr,&len);
    if(cfd == -1) {
        perror("accept");
        exit(-1);
    }
    printf("client socket filename:%s\n",cliaddr.sun_path);

    while(1) {
        char buf[128];
        int len = recv(cfd,buf,sizeof(buf),0);
        if(len == -1) {
            perror("recv");
            exit(-1);
        }else if(len == 0) {
            printf("client close ....\n");
            break;
        }else{
            printf("client recv: %s\n",buf);
            send(cfd,buf,len,0);
        }
    }
    close(cfd);
    close(lfd);
    return 0;
}