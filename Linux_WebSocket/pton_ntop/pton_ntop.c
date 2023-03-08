#include <stdio.h>
#include <arpa/inet.h>

int main(void) {
    //将点分十进制转为网络字节序的整数
    char buf[] = "192.168.1.1";
    unsigned int num = 0;
    inet_pton(AF_INET,buf,&num);
    unsigned char* p = (unsigned char*)&num;
    printf("%d %d %d %d\n",*p,*(p+1),*(p+2),*(p+3));

    //将网络字节序转为点分十进制·
    char ip[16] = "";
    const char* str= inet_ntop(AF_INET,&num,ip,sizeof(ip));
    printf("str = %s\n",str);
    printf("ip = %s\n", ip);
    printf("str == ip ? %d", str == ip);
    return 0;
}