#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <string.h>
#include <sys/epoll.h>
#include <unistd.h>

int main() {
    //创建sockt监听客户端监听文件描述符
    int lfd = socket(AF_INET,SOCK_STREAM,0);

    //绑定
    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = htons(9999);
    saddr.sin_addr.s_addr = INADDR_ANY;
    bind(lfd,(struct sockaddr*)&saddr,sizeof(saddr));

    //监听
    listen(lfd,9);

    //创建epoll实例
    int epfd = epoll_create(100);
    
    //将监听的文件描述符加入epoll实例中，epev文件描述符相关信息
    struct epoll_event epev;
    epev.data.fd = lfd;
    epev.events = EPOLLIN;
    epoll_ctl(epfd,EPOLL_CTL_ADD,lfd,&epev);

    struct epoll_event epevs[1024];

    while(1) {
        //检测函数，返回值为变化的文件描述符个数
        int ret = epoll_wait(epfd,epevs,1024,-1);
        if(ret == -1) {
            perror("epoll");
            exit(-1);
        }
        printf("ret = %d\n", ret);
        
        for(int i = 0; i < ret; i++) {
            //监听的文件描述符有数据到达，说明又客户端连接
            int curfd = epevs[i].data.fd;
            if(curfd == lfd) {
                struct sockaddr_in cliaddr;
                int len = sizeof(cliaddr);
                int cfd = accept(lfd,(struct sockaddr*)&cliaddr,&len);
                
                //将客户端描述符添加到epoll实例中
                epev.data.fd = cfd;
                epev.events = EPOLLIN;
                epoll_ctl(epfd,EPOLL_CTL_ADD,cfd,&epev);
            }else{
                if(epevs[i].events & EPOLLOUT) {
                    continue;
                }
                char buf[1024] = {0};
                int len = read(curfd, buf, sizeof(buf));
                if(len == -1) {
                    perror("read");
                    exit(-1);
                } else if(len == 0) {
                    printf("client closed...\n");
                    epoll_ctl(epfd, EPOLL_CTL_DEL, curfd, NULL);
                    close(curfd);
                } else if(len > 0) {
                    printf("read buf = %s\n", buf);
                    write(curfd, buf, strlen(buf) + 1);
                }
            }
        }

    }
    close(lfd);
    close(epfd);
    return 0;
}