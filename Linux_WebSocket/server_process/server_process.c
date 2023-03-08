#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <signal.h>
#include <wait.h>
#include <errno.h>

void recycleChild(int arg) {
    while(1) {
        int ret = waitpid(-1,NULL,WNOHANG);
        if(ret == -1) {
            break;
        }else if(ret == 0) {
            break;
        }else if(ret > 0) {
            printf("子进程 %d 被回收了\n",ret);
        }
    }
}


int main() {
    struct sigaction act;
    act.sa_flags = 0;
    sigemptyset(&act.sa_mask);
    act.sa_handler = recycleChild;
    sigaction(SIGCHLD,&act,NULL);


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

    while(1) {
        //4.等待客户端连接
        struct sockaddr_in cliaddr;
        int len = sizeof(cliaddr);
        int cfd = accept(lfd,(struct sockaddr*)&cliaddr,&len);
        if(cfd == -1) {
            if(errno == EINTR) {
                continue;
            }
            perror("accept");
            exit(-1);
        }

        pid_t pid = fork();
        if(pid == 0) {
            //子进程
             //5.输出客户端信息
            char buf[16];
            inet_ntop(PF_INET,&cliaddr.sin_addr.s_addr,buf,sizeof(buf));
            unsigned short cliPort = ntohs(cliaddr.sin_port);
            printf("client ip: %s, port is %d\n",buf,cliPort);

            //6.通信
            char recvBuf[1024];
            while(1) {
                memset(recvBuf,0,sizeof(recvBuf));
                int num = read(cfd,recvBuf,sizeof(recvBuf));
                if(num == -1) {
                    perror("read");
                    exit(-1); 
                }else if(num > 0){
                    printf("server read data:%s\n",recvBuf);
                }else if(num == 0) {
                    printf("client closed\n");
                    break;
                }

                write(cfd,recvBuf,strlen(recvBuf) + 1);
            }
            close(cfd);
            exit(0);
        }
    }
    
    
    close(lfd);
    return 0;
}