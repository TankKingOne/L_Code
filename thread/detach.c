#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

void *thr_start(void *arg)
{
  pthread_detach(pthread_self());
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
  // pthread_detach(tid);
  while(1)
  {
    printf("retval : %s\n", retval);
    sleep(1);
  }
  return 0;
}
