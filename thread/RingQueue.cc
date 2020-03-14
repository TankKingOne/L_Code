#include <iostream>
#include <vector>
#include <pthread.h>
#include <semaphore.h>
#include <cstdio>

#define MAX_QUEUE 5
#define MAX_THREAD 5

class RingQueue{
  private:
    std::vector<int> _queue;
    int _capcity;
    int _step_read; // 当前读取位置下标
    int _step_write; // 当前写入位置下标

    sem_t _sem_lock; // 用于实现互斥的信号量
    sem_t _sem_data; // 对当前队列中的数据资源的数量进行计数；如果<=0表示没有资源，则会陷入等待
    sem_t _sem_space; // 对当前队列中空闲空间数量进行计数；如果<=0表示队列满了，则生产者陷入等待

  public:
    RingQueue(int max = MAX_QUEUE): _queue(max), _step_read(0), _step_write(0){
     sem_init(&_sem_lock, 0, 1); // 0表示用于线程间；初值为1
     sem_init(&_sem_data, 0, 0); // 0:没有数据
     sem_init(&_sem_space, 0, max); // max:全部都是空
    }
  ~RingQueue(){
    sem_destroy(&_sem_lock);
    sem_destroy(&_sem_data);
    sem_destroy(&_sem_space);
  }
  bool Push(int data){
    sem_wait(&_sem_space); // 统计空间节点数量，自动判断 是否有空闲空间，没有则阻塞
    sem_wait(&_sem_lock); // 若先加锁再判断，有可能会陷入休眠而没解锁

    _queue[_step_write] = data;
    _step_write = (_step_write + 1) % _capcity;

    sem_post(&_sem_lock);
    sem_post(&_sem_data); // 入队数据之后，数据资源的数量增加一个，并且唤醒消费者

    return true;
  }
  bool Pop(int *data){
    sem_wait(&_sem_data); // 消费者判断数据资源数量，若<=0则陷入等待，计数-1
    sem_wait(&_sem_lock);

    *data = _queue[_step_read];
    _step_read = (_step_read + 1) % _capcity;

    sem_post(&_sem_lock);
    sem_post(&_sem_space); // 空间资源多了一个，唤醒生产者

    return true;
  }
};

void *thr_consumer(void *arg){
  RingQueue *r = (RingQueue*)arg;
  while(1){
    int data;
    r->Pop(&data);
    printf("Consumer : %p data : %d\n", pthread_self(), data);
  }
}

void *thr_productor(void *arg){
  RingQueue *r = (RingQueue*)arg;
  int i = 0;
  while(1){
    r->Push(i);
    printf("Productor : %p data : %d\n", pthread_self(), i++);
  }
  return NULL;
}

int main()
{
  pthread_t ctid[MAX_THREAD], ptid[MAX_THREAD];
  int ret, i;
  RingQueue r;

  // 创建消费者线程
  for(i = 0; i < MAX_THREAD; i++){
    ret = pthread_create(&ctid[i], NULL, thr_consumer, (void*)&r);
    if(ret != 0){
      printf("thread creat error!\n");
      return -1;
    }
  }
  // 创建生产者线程
  for(i = 0; i < MAX_THREAD; i++){
    ret = pthread_create(&ptid[i], NULL, thr_productor, (void*)&r);
    if(ret != 0){
      printf("thread creat error!\n");
      return -1;
    }
  }
  // 等待所有线程退出
  for(i = 0; i < MAX_THREAD; i++){
    pthread_join(ctid[i], NULL);
    pthread_join(ptid[i], NULL);
    }
  return 0;
}
