#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <string>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstring>
#include <stdlib.h>

void Usage(std::string proc)
{
  std::cerr << "Usage:" << proc << "  IP  PORT" << std::endl;
  std::cerr << "Version:0.0.1" << std::endl;
}

int main(int argc, char *argv[])
{
  if(argc != 3)
  {
    Usage(argv[0]);
    return 1;
  }
  int sock = socket(AF_INET, SOCK_DGRAM, 0);
  if(sock < 0)
  {
    std::cerr << "socket error" << std::endl;
    return 2;
  }
  std::cout << "sock: " << sock << std::endl;
  struct sockaddr_in local;
  local.sin_family = AF_INET;
  local.sin_port = htons(atoi(argv[2]));
  local.sin_addr.s_addr = inet_addr(argv[1]);
  if(bind(sock, (struct sockaddr*)&local, sizeof(local)) < 0)
  {
    std::cerr << "bind error" << std::endl;
    return 3;
  }

  char buf[1024];
  for(;;)
  {
    struct sockaddr_in peer;
    socklen_t len = sizeof(peer);
    ssize_t s = recvfrom(sock, buf, sizeof(buf)-1, 0, (struct sockaddr*)&peer, &len);
    if(s > 0)
    {
      buf[s] = 0;
      std::cout << "client# " << buf << std::endl;
      sendto(sock, buf, strlen(buf), 0, (struct sockaddr*)&peer, len);
    }
  }

  close(sock);
  return 0;
}
