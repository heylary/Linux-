#ifndef LOCKER_H
#define LOCKER_H

#include <pthread.h>
#include <exception>
#include <semaphore.h>
/*
这段代码实现了对线程同步机制的封装，主要用于多线程编程中的同步和互斥操作。具体作用如下：

locker 类：提供互斥锁的封装，保证在多线程环境下访问共享资源的安全性。
cond 类：提供条件变量的封装，用于多个线程之间的协调和通信，可以实现线程的等待、唤醒和广播等功能。
sem 类：提供信号量的封装，用于解决资源竞争问题，在多线程共享资源时，通过控制信号量来实现线程间的同步和互斥。
这些同步机制的封装使得多线程编程更加简单和高效，避免了直接使用底层 API 导致的错误和不便。同时也提高了代码的可读性、可维护性和可移植性。
*/

//线程同步机制封装类
// 这段代码实现了一个线程同步机制封装类，包含互斥锁、条件变量和信号量三种同步方式。具体包含以下内容：

// locker 类：封装了互斥锁操作，包括构造函数初始化互斥锁、析构函数销毁互斥锁、lock() 上锁、unlock() 解锁和 get() 获取互斥锁对象的地址。
// cond 类：封装了条件变量操作，包括构造函数初始化条件变量、析构函数销毁条件变量、wait() 等待条件变量、timedwait() 带超时等待条件变量、signal() 发送条件变量信号和 broadcast() 广播条件变量信号。
// sem 类：封装了信号量操作，包括构造函数初始化信号量、析构函数销毁信号量、wait() 等待信号量和 post() 发送信号量。
// 在实现过程中，使用了 pthread 库提供的线程相关函数 pthread_mutex_init、pthread_mutex_destroy、pthread_mutex_lock、
// pthread_mutex_unlock、pthread_cond_init、pthread_cond_wait、pthread_cond_timedwait、pthread_cond_signal、
// pthread_cond_broadcast、sem_init、sem_destroy、sem_wait 和 sem_post。同时也使用了 C++ 异常机制来处理异常。

//互斥锁类
class locker{
    public:
        locker() {
            if(pthread_mutex_init(&m_mutex,NULL) != 0) {
                throw std::exception();
            }
        }

        ~locker() {
            pthread_mutex_destroy(&m_mutex);
        }

        bool lock() {
            return pthread_mutex_lock(&m_mutex) == 0;
        }

        bool unlock() {
            return pthread_mutex_unlock(&m_mutex) == 0;
        }

        pthread_mutex_t* get() {
            return &m_mutex;
        }

    private:
        pthread_mutex_t m_mutex;
};

//条件变量类
class cond {
    public:
    cond() {
        if(pthread_cond_init(&m_cond,NULL) != 0) {
            throw std::exception();
        }
    }

    ~cond() {
        pthread_cond_destroy(&m_cond);
    }

    bool wait(pthread_mutex_t *m_mutex) {
        return pthread_cond_wait(&m_cond,m_mutex) == 0;
    }

    bool timedwait(pthread_mutex_t *m_mutex,struct timespec t) {
        return pthread_cond_timedwait(&m_cond,m_mutex,&t) == 0;
    }

    bool signal() {
        return pthread_cond_signal(&m_cond) == 0;
    }

    bool broadcast() {
        return pthread_cond_broadcast(&m_cond) == 0;
    }


    private:
        pthread_cond_t m_cond;
};

//信号量类
class sem {
    public:
        sem() {
            if(sem_init(&m_sem,0,0) != 0) {
                throw std::exception();
            }
        }

        sem(int num) {
            if(sem_init(&m_sem,0,num) != 0) {
                throw std::exception();
            }
        }

        ~sem() {
            sem_destroy(&m_sem);
        }

        //等待信号量
        bool wait() {
            return sem_wait(&m_sem) == 0;
        }

        //增加信号量
        bool post() {
            return sem_post(&m_sem) == 0;
        }

    private:
        sem_t m_sem;
};

#endif