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
      sleep(1);
    }
    exit(0);
  }
  else // father
  {
    close(fd[1]);
    char buf[1024];
    while(1)
    {
      sleep(5);
      ssize_t s = read(fd[0], buf, sizeof(buf) - 1);
      if(s > 0)
      {
        buf[s] = 0;
        printf("%s\n", buf);
      }
      else if(s == 0)
      {
        printf("read file end!\n");
        break;
      }
      else 
      {
        printf("read error!\n");
        break;
      }
      break;
    }
    close(fd[0]);
    int status = 0;
    waitpid(id, &status, 0);
    printf("sig : %d\n", status & 0x7F); // 退出码，低7位
  }
  return 0;
}
