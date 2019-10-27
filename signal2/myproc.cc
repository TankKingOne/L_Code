#include <iostream>
#include <sys/types.h>
#include <signal.h>

void handler(int signo)
{
  std::cout << "get a signo : " << signo << std::endl;
}

int main()
{
  struct sigaction act, oact;
  act.sa_handler = handler;
  sigemptyset(&act.sa_mask);
  act.sa_flags = 0;

  sigaction(2, &act, &oact);
  while(1);
  return 0;
}
