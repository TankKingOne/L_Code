#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

int g_tickets = 100;

// 1.定义互斥锁变量，并且这个变量也额是一个临界资源，需要能够被各个线程访问到
// 可以将互斥锁变量定义为一个全局变量，也可以将其定义成局部变量，然后通过函数传参传递给线程
pthread_mutex_t mutex;

void *thr_Scalpers(void *arg)
{
  // 尽量避免对不需要加锁的操作进行加锁，会影响效率，因为在加锁期间别人都不能操作
  // 若加锁的操作越多，意味着需要的时间就更长
  while(1)
  {
    pthread_mutex_lock(&mutex); // 加锁一定是在临界资源访问之前，保护的也仅仅是临界区
    if(g_tickets > 0)
    {
      usleep(1000);
      printf("I : [%p] got a ticket : %d\n", pthread_self(), g_tickets);
      g_tickets--;

      // 解锁是在临界资源访问完毕之后
      pthread_mutex_unlock(&mutex);
      usleep(1000); // 防止一个黄牛抢完马上又抢（一个线程分到时间片，解锁后立马又加锁）
    }
    else 
    {
      // 在任意有可能退出线程的地方，记得解锁
      // 若退出没有解锁，则其他线程获取不到锁，就会卡死
      pthread_mutex_unlock(&mutex);
      pthread_exit(NULL);
    }
  }
  return NULL;
}

int main()
{
  int i = 0;
  pthread_t tid[4];
  // 2.初始化互斥锁，一定要在创建线程之前！
  pthread_mutex_init(&mutex, NULL);

  for(i = 0; i < 4; ++i)
  {
    int ret = pthread_create(&tid[i], NULL, thr_Scalpers, NULL);
    if(ret != 0)
    {
      printf("thread create error!\n");
      return -1;
    }
  }
  for(i = 0; i < 4; ++i)
  {
    pthread_join(tid[i], NULL);
  }
  // 3.销毁互斥锁，释放资源
  pthread_mutex_destroy(&mutex);
  return 0;
}
