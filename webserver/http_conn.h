#ifndef HTTPCONNECTION_H
#define HTTPCONNECTION_H

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

class http_conn {
    public:
        static int m_epollfd; //所有的socket事件都被注册到同一个epoll对象上
        static int m_user_count; //统计用户的数量
        http_conn() {}
        ~http_conn() {}

        void process(); //处理客户端的请求 
        void init(int sockfd,const sockaddr_in &addr);  //初始化新接收的连接
        void close_conn(); //关闭连接
        bool read(); //非阻塞的读
        bool write(); //非阻塞的写
    private:
        int m_sockfd; //该http连接的socket
        sockaddr_in m_address;  //通信的socket地址
        

};

#endif