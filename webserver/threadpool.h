#ifndef THREADPOOL_H
#define THREADPOOL_H


/*

构造函数：初始化线程池大小和最大请求队列长度，并为每个工作线程创建线程并设置为脱离状态。
析构函数：销毁线程池，同时设置 m_stop 标记为 true 表示结束线程。
append() 函数：向工作队列中添加任务，并通过信号量来通知有任务需要处理。如果队列已满则添加任务失败。
worker() 函数：线程池中所有工作线程的入口函数，调用 run() 函数来处理任务。
run() 函数：从工作队列中获取任务并执行，如果队列为空则等待。该函数包含以下操作：
等待信号量 m_queuestat 的触发，即是否有任务需要处理。
使用互斥锁 m_queuelocker 来保护工作队列，避免多个线程同时访问队列导致数据不一致。
获取队首任务并弹出，然后释放互斥锁，开始对任务进行处理。
如果获取到的任务为空，则跳过本次循环继续下一次等待。
否则，调用任务的 process() 函数来处理任务。
此外，还使用了自己实现的 locker 类和 sem 类封装了互斥锁和信号量等同步机制，以确保线程安全性和避免竞态条件。

*/

#include <pthread.h>
#include <list>
#include "locker.h"
#include <exception>
#include <cstdio>
#include <iostream>


//线程池类、定义成模板类是为了代码的复用，模板参数T是任务类
template<typename T>
class threadpool {
    public:
        threadpool(int thread_number = 8,int max_request = 10000);
        ~threadpool();

        //往工作队列中添加任务
        bool append(T* request);
    private:
        static void* worker(void * arg);
        void run();

    private:
        //线程的数量
        int m_thread_number;
        //线程池数组，大小为m_pthread_number
        pthread_t *m_threads;

        //请求队列中最多允许的，等待处理的数量
        int m_max_requests;
        //请求队列
        std::list<T *> m_workqueue;

        //互斥锁
        locker m_queuelocker;

        //信号量,用来判断是否有任务需要处理
        sem m_queuestat;

        //是否结束线程
        bool m_stop;
};

//构造函数
template <typename T> 
threadpool<T>::threadpool(int thread_number,int max_request):
    m_thread_number(thread_number),m_max_requests(max_request),
    m_stop(false),m_threads(NULL) {
        if(thread_number <= 0 || max_request <= 0) {
            throw std::exception();
        } 

        //创建线程
        m_threads = new pthread_t(m_thread_number); 
        if(!m_threads) {
            throw std::exception();
        }
        //创建thread_number个线程，并将它们设置为线程脱离
        for(int i = 0; i < thread_number; ++i) {
            printf("create the  %dth thread\n",i );
            if(pthread_create(m_threads + i, NULL, worker, this) != 0) {
                delete []m_threads;
                throw std::exception();
            }

            if(pthread_detach(m_threads[i])) {
                delete[]m_threads;
                throw std::exception();
            }
        }
    }

template<typename T>
threadpool<T>::~threadpool() {
    delete m_threads;
    m_stop = true;
}

template<typename T>
bool threadpool<T>::append(T* request) {
    m_queuelocker.lock();
    if(m_workqueue.size() > m_max_requests) {
        m_queuelocker.unlock();
        return false;
    }
    m_workqueue.push_back(request);
    m_queuelocker.unlock();
    m_queuestat.post();
    return true;
}

template<typename T>
void * threadpool<T>::worker(void * arg) {
    threadpool *pool = (threadpool *) arg;
    pool->run();
    return pool;
}

template<typename T>
void threadpool<T>::run() {
    while(!m_stop) {
        m_queuestat.wait();    //如果信号量有值
        m_queuelocker.lock();
        if(m_workqueue.empty()) {
            m_queuelocker.unlock();
            continue;
        }

        T* request= m_workqueue.front();
        m_workqueue.pop_front();
        m_queuelocker.unlock();

        if(!request) {  //获取到了，继续往下走
            continue;
        }

        request->process();
    }
}


#endif