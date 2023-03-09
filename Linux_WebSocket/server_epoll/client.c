#include <stdio.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int main(void) {
    int fd = socket(AF_INET,SOCK_STREAM,0);

    struct sockaddr_in saddr;
    saddr.sin_family = AF_INET;
    saddr.sin_port = ntohs(9999);
    inet_pton(AF_INET,"127.0.0.1",&saddr.sin_addr.s_addr);

    int ret = connect(fd,(struct sockaddr*)&saddr,sizeof(saddr));

    if(ret == -1) {
        perror("connect");
        exit(-1);
    }

    int num = 0;
    while(1) {
        char sendBuf[1024] = {0};
        sprintf(sendBuf,"client send data:%d",num++);
        write(fd,sendBuf,strlen(sendBuf) + 1);
        // 接收
        int len = read(fd, sendBuf, sizeof(sendBuf));
        if(len == -1) {
            perror("read");
            return -1;
        }else if(len > 0) {
            printf("read buf = %s\n", sendBuf);
        } else {
            printf("服务器已经断开连接...\n");
            break;
        }
        // sleep(1);
        usleep(1000);
    }
    close(fd);
    return 0;
}