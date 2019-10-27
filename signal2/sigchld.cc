#include <iostream>
#include <sys/types.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>

void handler(int signo)
{
  std::cout << "id : " << getpid() << std::endl;
  std::cout << "signo : " << signo << std::endl;
}

int main()
{
  signal(SIGCHLD, handler);
  if(fork() == 0)
  {
    std::cout << "Child id : " << getpid() << std::endl;
    sleep(5);
    exit(1);
  }
  else 
  {
    std:: cout << "father id : " << getpid() << std::endl;
    while(1);
  }
  return 0;
}
