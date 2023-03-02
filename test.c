#include <stdio.h>

int main() {
    int a = 10;
    int *b = &a;
    printf("a result: %d \n",a);    //a的值
    printf("&a result: %d \n",&a);  //a的地址
    printf("b result: %d \n",b);    //指针直接用名字代表首地址
    printf("*b result: %d \n",*b);  //取值
    printf("&b result: %d \n",&b);  //指针的地址
    return 0;
}