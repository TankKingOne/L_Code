#ifndef _TCP_SERVER_HPP
#define _TCP_SERVER_HPP

#include <iostream>
#include <string>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

class TcpServer
{
private:
  int listen_sock;
  std::string ip;
  int port;
public:
  TcpServer(std::string ip_, int port_):ip(ip_), port(port_), listen_sock(-1)
    {}
  void InitServer()
  {
    listen_sock = socket(AF_INET, SOCK_STREAM, 0);
    if(listen_sock < 0)
    {
      std::cerr << "socket error" << std::endl;
      exit(2);
    }
    struct sockaddr_in local;
    bzero(&local, sizeof(local));
    local.sin_family = AF_INET;
    local.sin_port = htons(port);
    local.sin_addr.s_addr = inet_addr(ip.c_str());
    if(bind(listen_sock, (struct sockaddr*)&local, sizeof(local)) < 0)
    {
      std::cerr << "bind error" << std::endl;
      exit(3);
    }
    if(listen(listen_sock, 5) < 0)
    {
      std::cerr << "listen error" << std::endl;
      exit(4);
    }
  }
  void Service(int sock)
  {
    while(1)
    {
      char buf[1024];
      ssize_t s = read(sock, buf, sizeof(buf)-1);
      if(s > 0)
      {
        buf[s] = 0;
        std::cout << "client# " << buf << std::endl;
        write(sock, buf, strlen(buf));
      }
      else if(s == 0)
      {
        std::cout << "client quit!" << std::endl;
        break;
      }
      else 
      {
        std::cerr << "read error" << std::endl;
        break;
      }
    }
    close(sock);
  }
  void Start()
  {
    struct sockaddr_in peer;
    socklen_t len;
    for(;;)
    {
      int sock = accept(listen_sock, (struct sockaddr*)&peer, &len);
      if(sock < 0)
      {
        std::cerr << "accept error" << std::endl;
        continue;
      }
      std::cout << " Get a new client " << sock << std::endl;
      Service(sock);
    }
  }
  ~TcpServer()
  {
    if(listen_sock > 0)
    close(listen_sock);
  }
};
#endif


