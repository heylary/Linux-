#include "http_conn.h"
#include <sys/epoll.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <stdarg.h>
#include <error.h>
#include "locker.h"
#include <sys/uio.h>

//添加文件描述符到epoll中
void addfd(int epollfd,int fd, bool one_shot) {
    epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLRDHUP; 

    if(one_shot) {
        event.events | EPOLLONESHOT;
    }

    epoll_ctl(epollfd,EPOLL_CTL_ADD,fd,&event);
}

//从epoll中删除文件描述符
void removefd(int epollfd,int fd) {
    epoll_ctl(epollfd,EPOLL_CTL_DEL,fd,0);
    close(fd);
}

//修改epoll文件描述符,重置socket上的EPOLLONESHOT事件，以确保下一次可读时，EPOLLIN时间能被触发
void modfd(int epollfd,int fd,int ev) {
    epoll_event event;
    event.data.fd = fd;
    event.events = ev | EPOLLONESHOT | EPOLLRDHUP;
    epoll_ctl(epollfd,EPOLL_CTL_MOD,fd,&event);
}