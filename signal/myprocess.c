#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>

void show(sigset_t *pending)
{
  int i = 1;
  for(; i <= 31; i++)
  {
    if(sigismember(pending, i))
    {
      printf("1");
    }
    else 
    {
      printf("0");
    }
  }
  printf("\n");
}

void handler(int sig)
{
  printf("handler : %d\n", sig);
}

int main()
{
  signal(2, handler);

  sigset_t set, oset;
  sigemptyset(&set);
  sigemptyset(&oset);

  sigaddset(&set, 2);
  sigprocmask(SIG_SETMASK, &set, &set);

  int count = 10;
  while(1)
  {
    sigset_t pending;
    sigemptyset(&pending);
    sigpending(&pending);
    show(&pending);
    sleep(1);
    if(count-- <= 0)
    {
      printf("recover signal!\n");
      sigprocmask(SIG_SETMASK, &oset, NULL);
    }
  }
  return 0;
}
//int count = 0;
//
//void handler(int signo)
//{
//  printf("catch sig, signo : %d, count : %d\n", signo, count);
//  exit(1);
//}
//
//int main()
//{
//  int i = 1;
//  while(i <= 31)
//  {
//    signal(i++, handler);
//  }
//  sleep(3);
//  int a = 10;
//  a /= 0;
//  while(1);
//
//  return 0;
//}
