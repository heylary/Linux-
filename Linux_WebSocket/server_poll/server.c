#include <stdlib.h>
#include <stdio.h>
#include <poll.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <string.h>

int main() {

    int lfd = socket(AF_INET,SOCK_STREAM,0);

    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = ntohs(9999);
    saddr.sin_addr.s_addr = INADDR_ANY;

    bind(lfd,(struct sockaddr*)&saddr,sizeof(saddr));

    listen(lfd,9);

    struct pollfd fds[1024];
    for(int i = 0; i < 1024; i++) {
        fds[i].fd = -1;
        fds[i].events = POLLIN;
    }

    fds[0].fd = lfd;
    int nfds = 0;
    while(1) {
        int ret = poll(fds,nfds + 1,-1);
        if(ret == -1) {
            perror("poll");
            exit(-1);
        }else if(ret > 0) {
            if(fds[0].revents & POLLIN) {
                struct sockaddr_in cliaddr;
                unsigned short cliPort = ntohs(cliaddr.sin_port);
                char cliIp[16];
                inet_ntop(AF_INET,&cliaddr.sin_addr.s_addr,cliIp,sizeof(cliIp));
                printf("client ip: %s,port: %d\n",cliIp,cliPort);

                int len = sizeof(cliaddr);
                int cfd = accept(lfd,(struct sockaddr*)&cliaddr,&len);
                //将新文件描述符加入到集合中
                for(int i = 1; i < 1024; i++) {
                    if(fds[i].fd == -1) {
                        fds[i].fd = cfd;
                        fds[i].events = POLLIN;
                        break;;
                    }
                }
                //更新最大的文件描述符
                nfds = nfds > cfd ? nfds : cfd;
            }
                //通信
                for(int i = 1; i < nfds; i++) {
                    if(fds[i].revents & POLLIN) {
                        char buf[1024] = {0};
                        int len = read(fds[i].fd,buf,sizeof(buf));
                        if(len == -1) {
                            perror("read");
                            exit(-1);
                        }else if(len == 0) {
                            printf("close client...");
                            close(fds[i].fd);
                            fds[i].fd = -1;
                        }else if(len > 0) {
                            printf("read buf: %s\n",buf);
                            write(fds[i].fd,buf,strlen(buf)+1);
                        }
                    } 
                }
        }

    }
    close(lfd);
    return 0;
}