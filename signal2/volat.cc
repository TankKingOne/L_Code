#include <iostream>
#include <sys/types.h>
#include <signal.h>

volatile bool quit = false;

void handler(int signo)
{
  switch(signo)
  {
    case 2:
      quit = true;
      std::cout << "hello world, quit : " << quit << std::endl;
      break;
    default:
      break;
  }
}

int main()
{
  signal(2, handler);
  std::cout << "while quit " << quit << std::endl;
  while(!quit);
  std::cout << "while quit!" << std::endl;
  return 0;
}
