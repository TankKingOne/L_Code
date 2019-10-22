#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <stdlib.h>

int main()
{
  int fd[2] = {0};
  pipe(fd);
  pid_t id = fork();
  if(id == 0)
  { // child
    close(fd[0]);
    const char *str = "I am child";
    int count = 0;
    while(1)
    {
      write(fd[1], str, strlen(str));
      printf("%d\n", count += strlen(str));
    }
    exit(0);
  }
  else // father
  {
    close(fd[1]);
    waitpid(id, NULL, 0);
  }
  return 0;
}
