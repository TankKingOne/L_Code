#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <signal.h>
#include <stdlib.h>

// kill 9 1234
int main(int argc, char* argv[])
{
  if(argc != 3)
  {
    printf("Usage : %s sig pid\n", argv[0]);
    return 1;
  }

  int sig = atoi(argv[1]);
  int pid = atoi(argv[2]);

  kill(pid, sig);
}
