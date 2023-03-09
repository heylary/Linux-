#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <sys/types.h>

int main() {

    int lfd = socket(PF_INET,SOCK_STREAM,0);
    if(lfd == -1) {
        perror("socket");
        exit(-1);
    }  

    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(9999);
    saddr.sin_addr.s_addr = INADDR_ANY;

    int ret = bind(lfd,(struct sockaddr*)&saddr,sizeof(saddr));
    if(ret == -1) {
        perror("bind");
        exit(-1);
    }

    listen(lfd,9);

    fd_set rdset,tmp;
    FD_ZERO(&rdset);
    FD_SET(lfd,&rdset);
    int maxfd = lfd;
    
    while(1) {
        tmp = rdset;
        int num = select(maxfd+1,&tmp,NULL,NULL,NULL);
        if(num == -1) {
            perror("select");
            exit(-1);
        }else if(num > 0) {
            if(FD_ISSET(lfd,&tmp)) {
                //开始accept，表示有客户端进入，开始接收
                struct sockaddr_in cliaddr;
                int len = sizeof(cliaddr);
                int cfd = accept(lfd,(struct sockaddr *)&cliaddr,&len);
                //将cfd加入集合中
                FD_SET(cfd,&rdset);
                maxfd = maxfd > cfd ? maxfd:cfd;
                //输出客户端信息
                char buf[16];
                inet_ntop(AF_INET,&cliaddr.sin_addr.s_addr,buf,sizeof(buf));
                unsigned short cliPort = ntohs(cliaddr.sin_port);
                printf("client ip: %s, port: %d.\n",buf,cliPort);
            }
                //通信
                for(int i = lfd + 1; i <= maxfd; i++) {
                    if(FD_ISSET(i,&tmp)) {
                        char recvbuf[1024] = {0};

                        int size = read(i,recvbuf,sizeof(recvbuf));
                        if(size == -1) {
                            perror("read");
                            exit(-1);
                        }else if(size == 0){
                            printf("close client...\n");
                            close(i);
                            FD_CLR(i,&rdset);
                        }else if(size > 0) {
                            printf("read buf = %s\n",recvbuf);
                            write(i,recvbuf,strlen(recvbuf) + 1);
                        }
                    }
                }
        }

    }
    close(lfd);

    return 0;
}