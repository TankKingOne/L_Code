#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

void *thr_start(void *arg)
{
  printf("normal thread will exit in 3 seconds\n");
  sleep(3);
  char *buf = "normal thread exit... \n"; // 返回的是常量的地址
  // char buf[] = "normal thread exit... \n"; // 返回的是局部空间的地址
  return buf;
}

int main()
{
  pthread_t tid;
  int ret = pthread_create(&tid, NULL, thr_start, NULL);
  if(ret != 0)
  {
    printf("thread create failed!\n");
    return -1;
  }
  printf("Main thread waiting for normal thread to exit\n");
  char *retval = NULL;
  pthread_join(tid, (void**)&retval); // 等待一个线程的退出，若线程没有退出则会一直等待（阻塞）
  printf("retval : %s", retval);
  return 0;
}
