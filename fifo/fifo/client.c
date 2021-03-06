#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

int main()
{
  int fd = open("fifo", O_WRONLY);
  if(fd < 0)
  {
    printf("open error\n");
    return 1;
  }
  char buf[1024];
  while(1)
  {
    printf("Please Enter Your Message : ");
    fflush(stdout);
    ssize_t s = read(0, buf, sizeof(buf) - 1);
    if(s > 0)
    {
      buf[s - 1] = 0;
      write(fd, buf, strlen(buf));
    }
    else if(s == 0)
    {
      printf("client quit!\n");
      break;
    }
    else 
    {
      printf("read error!\n");
      break;
    }
  }
  return 0;
}
