#include "TcpServer.hpp"

void Usage(std::string proc)
{
  std::cout << "Usage: " << proc << " port" << std::endl;
}

int main(int argc, char *argv[])
{
  if(argc != 2)
  {
    Usage(argv[0]);
    exit(1);
  }
  dict.insert(std::pair<std::string, std::string>("apple", "PingGuo"));
  dict.insert(std::pair<std::string, std::string>("banana", "XiangJiao"));
  dict.insert(std::pair<std::string, std::string>("peach", "TaoZi"));
  dict.insert(std::pair<std::string, std::string>("lemon", "NingMeng"));

  int port = atoi(argv[1]);
  TcpServer *tsp = new TcpServer(port);
  tsp->InitServer();
  tsp->Start();
  return 0;
}
