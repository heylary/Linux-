#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <stdlib.h>

void myFun(int num) {
    printf("被捕捉到的信号%d\n",num);

    while(1) {
        int ret = waitpid(-1,NULL,WNOHANG);
        if(ret > 0) {
                printf("进程%d被杀死\n",getpid());
            }else if(ret == -1) {
                break;
            }else {
                break;
        }
    }
    
}


int main() {
    //再信号捕捉产生之前先阻塞信号集
    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set,SIGCHLD);
    sigprocmask(SIG_BLOCK,&set,NULL);

    //创建20个子进程
    pid_t pid;
    for(int i = 0; i < 20; i++) {
        pid = fork();
        if(pid == 0) {
            break;
        }
    }

    if(pid > 0) {
        struct sigaction act;
        act.sa_flags = 0;
        act.sa_handler = myFun;
        sigemptyset(&act.sa_mask);

        
        //信号捕捉SIGCHILD
        sigaction(SIGCHLD,&act,NULL);
        
        sigprocmask(SIG_UNBLOCK,&set,NULL);

        while(1) {
            printf("parent process pid : %d\n", getpid());
            sleep(2);
        }
    }else if(pid == 0) {
        printf("child process pid : %d\n", getpid());
    }



}