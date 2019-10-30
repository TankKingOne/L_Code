#ifndef _CP_HPP_
#define _CP_HPP_


#include <iostream>
#include <queue>
#include <pthread.h>
#include <unistd.h>

using namespace std;

class BlockQueue{
  private:
    queue<int> q;
    int cap; // 容量
    pthread_mutex_t lock; // 互斥锁
    pthread_cond_t full; // 控制变量
    pthread_cond_t empty; // 控制变量


    void LockQueue()
    {
      pthread_mutex_lock(&lock);
    }
    void UnlockQueue()
    {
      pthread_mutex_unlock(&lock);
    }
    bool QueueIsFull()
    {
      return q.size() == cap;
    }
    bool QueueIsEmpty()
    {
      return q.size() == 0;
    }
    void ProductWait()
    {
      pthread_cond_wait(&full, &lock);
    }
    void ConsumerWait()
    {
      pthread_cond_wait(&empty, &lock);
    }
    void SignalConsumer()
    {
      pthread_cond_signal(&empty);
    }
    void SignalProduct()
    {
      pthread_cond_signal(&full);
    }
  public:
    BlockQueue(int cap_ = 32):cap(cap_)
    {
      pthread_mutex_init(&lock, nullptr);
      pthread_cond_init(&full, nullptr);
      pthread_cond_init(&empty, nullptr);
    }
    void PushData(const int &in)
    {
      LockQueue();
      while(QueueIsFull())
      {
        SignalConsumer();
        ProductWait();
      }
      // product 
      q.push(in);
      SignalConsumer();
      UnlockQueue();
    }
    void PopData(int &out)
    {
      LockQueue();
      while(QueueIsEmpty())
      {
        SignalProduct();
        ConsumerWait();
      }
      // comsumer
      out = q.front();
      q.pop();
      SignalProduct();
      UnlockQueue();
    }
    ~BlockQueue()
    {
      pthread_mutex_destroy(&lock);
      pthread_cond_destroy(&full);
      pthread_cond_destroy(&empty);
    }
};


#endif
