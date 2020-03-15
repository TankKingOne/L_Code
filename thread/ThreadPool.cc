#include <cstdio>
#include <pthread.h>
#include <iostream>
#include <queue>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>

#define THREAD_COUNT 5

typedef void (*task_handler_t) (int arg);

class ThreadTask{
  private:
    int _data; // 要处理的数据
    task_handler_t _handler; // 处理数据的方法
  public:
    ThreadTask(){}
    ThreadTask(const int &data, task_handler_t handler):
    _data(data), _handler(handler){}
    void SetTask(const int &data, task_handler_t handler){
      _data = data;
      _handler = handler;
    }
    void Run(){
      return _handler(_data);
    }
};

class ThreadPool{
  private:
    int _thr_max; // 最大线程数量
    std::queue<ThreadTask> _queue;
    // 创建线程的时候，需要传入线程入口函数，导致线程池中的线程都是完成同一个任务的套路
    // 线程池中的线程不管什么数据，不管如何处理，只管调用任务对象中的Run接口
    pthread_mutex_t _mutex; // 用于实现队列操作的互斥
    pthread_cond_t _cond_pool; // 线程池中的线程都是消费者，若没有任务就阻塞
  public:
    ThreadPool(int thr_count = THREAD_COUNT):_thr_max(thr_count){
      pthread_mutex_init(&_mutex, NULL);
      pthread_cond_init(&_cond_pool, NULL);
      for(int i = 0; i < _thr_max; ++i){
        pthread_t tid;
        int ret = pthread_create(&tid, NULL, thread_routine, (void*)this);
        if(ret != 0){
          std::cerr << "thread create error!\n" << std::endl;
          exit(0);
        }
        pthread_detach(tid); // 线程分离--因为我们不关心线程的返回值
      }
    }
    ~ThreadPool(){
      pthread_mutex_destroy(&_mutex);
      pthread_cond_destroy(&_cond_pool);
    }
    bool TaskPush(const ThreadTask &task){
        pthread_mutex_lock(&_mutex);
        _queue.push(task);
        pthread_mutex_unlock(&_mutex);
        pthread_cond_signal(&_cond_pool); // 入队之后唤醒一下线程池中的线程
      return true;
    }
    void QueueLock(){
      pthread_mutex_lock(&_mutex);
    }
    bool QueueIsEmpty(){
      return _queue.empty();
    }
    void ThreadWait(){
      pthread_cond_wait(&_cond_pool, &_mutex);
    }
    bool TaskPop(ThreadTask *task){
      *task = _queue.front();
      _queue.pop();
      return true;
    }
    void QueueUnlock(){
      pthread_mutex_unlock(&_mutex);
    }
  private:
    // 这个函数如果是一个类成员函数，有一个隐藏参数this指针，因此需要定义成静态函数
    // 这是一个静态函数，没有this指针，导致无法直接访问类的内部成员变量
    // 通过参数传入当前的对象this指针，进而访问对象的公有成员变量以及成员函数
    static void *thread_routine(void *arg){
      ThreadPool *pool = (ThreadPool*)arg;
      // 循环从队列获取任务进行处理
      while(1){
        pool->QueueLock();
        while(pool->QueueIsEmpty()){
          pool->ThreadWait();
        }
        ThreadTask task;
        pool->TaskPop(&task);
        pool->QueueUnlock(); // 解锁在任务处理之前
        task.Run(); // 通过Run接口直接通过用户传入的方法完成数据处理
      }
      return NULL;
    }
};

void test(int data)
{
  srand(time(NULL));
  int sec = rand() % 5;
  printf("thread : %p : %d sleep %d sec\n", pthread_self(), data, sec);
  sleep(sec);
}

int main()
{
  ThreadPool pool;
  for(int i = 0; i < 10; i++){
    ThreadTask task;
    task.SetTask(i, test);
    pool.TaskPush(task);
  }
  while(1){
    sleep(1);
  }
  return 0;
}
