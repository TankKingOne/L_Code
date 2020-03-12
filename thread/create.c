#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

void *thread_start(void *arg)
{
  printf("accept information : %s", (char*)arg);
  while(1)
  {
    printf("this is a normal thread : %p\n", pthread_self());
    sleep(1);
  }
  return NULL;
}
// 程序运行起来之后，操作系统会创建一个主线程pcb调度运行main函数
// 在这个线程中可以创建另一个线程，创建一个pcb运行线程运行线程入口函数
int main()
{
  pthread_t tid;
  char *buf = "Hello thread~\n";
  int ret = pthread_create(&tid, NULL, thread_start, (void*)buf);
  if(ret != 0)
  {
    printf("Thread creation failed\n");
    return -1;
  }
  printf("normal thread id : %p\n", tid);
  while(1)
  {
    printf("this is main thread : %p\n", pthread_self());
    sleep(1);
  }
  return 0;
}
