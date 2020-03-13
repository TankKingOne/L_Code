#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

int bowl = 0; // 碗初始为0，表示没有饭

pthread_mutex_t mutex;
pthread_cond_t consumer_cond;
pthread_cond_t cook_cond;

void *thr_customer(void *arg)
{
  // 这是一个顾客流程
  while(1)
  {
    // 0.加锁
    pthread_mutex_lock(&mutex);
    while(bowl == 0)
    {
      // 如果没有饭，则要等待,因为已经加过锁了，所以等待之前要解锁，被唤醒之后要加锁
      // 因此pthread_cond_wait集合了三步操作：解锁/挂起/加锁
      // 解锁和挂起是一个原子操作-不可被打断
      // 顾客解锁，还没来得及挂起休眠，这时候厨师来做饭，做好后唤醒顾客（实际顾客还没休眠）
      pthread_cond_wait(&consumer_cond, &mutex);
    }
    printf("good tast!\n");
    bowl = 0; // 饭被吃完
    // 唤醒厨师，再做一碗
    pthread_cond_signal(&cook_cond);
    // 解锁操作
    pthread_mutex_unlock(&mutex);
  }
  return NULL;
}

void *thr_cook(void *arg)
{
  // 这是一个厨师流程
  while(1)
  {
    // 0.加锁操作，因为要对碗进行操作
    pthread_mutex_lock(&mutex);
    while(bowl == 1)
    {
      // 有饭，陷入等待
      pthread_cond_wait(&cook_cond, &mutex);
    }
    printf("cook...\n");
    bowl = 1; // 做了一碗饭
    // 唤醒顾客
    pthread_cond_signal(&consumer_cond);
    // 解锁操作
    pthread_mutex_unlock(&mutex);
  }
  return NULL;
}

int main()
{
  pthread_t ctid[2];
  int ret;
  int i = 0;

  pthread_mutex_init(&mutex, NULL);
  pthread_cond_init(&cook_cond, NULL);
  pthread_cond_init(&consumer_cond, NULL);

  for(i = 0; i < 4; ++i)
  {
    ret = pthread_create(&ctid[0], NULL, thr_customer, NULL);
    if(ret != 0)
    {
      printf("create customer failed!\n");
      return -1;
    }
  }


  for(i = 0; i < 4; ++i)
  {
    ret = pthread_create(&ctid[1], NULL, thr_cook, NULL);
    if(ret != 0)
    {
      printf("create cook failed!\n");
      return -1;
    }
  }

  pthread_join(ctid[0], NULL);
  pthread_join(ctid[1], NULL);

  pthread_mutex_destroy(&mutex);
  pthread_cond_destroy(&cook_cond);
  pthread_cond_destroy(&consumer_cond);
  return 0;
}
