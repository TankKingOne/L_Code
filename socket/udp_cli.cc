#include <iostream>
#include <unistd.h>
#include <string>
#include <stdlib.h>
#include <cstdio>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>

class UdpSocket{
  public:
    UdpSocket():_sockfd(-1){
    }
    // 1.创建套接字
    bool Socket(){
      _sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
      if(_sockfd < 0){
        perror("socket error!\n");
        return false;
      }
      return true;
    }
    // 2.为套接字绑定地址信息
    bool Bind(const std::string &ip, uint32_t port){
      // 1.定义IPv4地址结构
      struct sockaddr_in addr;
      addr.sin_family = AF_INET;
      addr.sin_port = htons(port);
      addr.sin_addr.s_addr = inet_addr(ip.c_str());
      // 2.绑定地址
      socklen_t len = sizeof(struct sockaddr_in);
      int ret = bind(_sockfd, (struct sockaddr*)&addr, len);
      if(ret < 0){
        perror("bind erroe!\n");
        return false;
      }
      return true;
    }
    // 3.发送数据
    bool Send(const std::string &data, const std::string &ip, uint16_t port){
      // 1.定义对端地址信息的IPv4地址结构
      struct sockaddr_in addr;
      addr.sin_family = AF_INET;
      addr.sin_port = htons(port);
      addr.sin_addr.s_addr = inet_addr(ip.c_str());
      // 2.向这个地址发送数据
      socklen_t len = sizeof(struct sockaddr_in);
      int ret = sendto(_sockfd, data.c_str(), data.size(), 0, (struct sockaddr*)&addr, len);
      if(ret < 0){
        perror("send error!\n");
        return false;
      }
      return true;
    }
    // 4.接收数据
    bool Recv(std::string *buf, std::string *ip = NULL, uint16_t *port = NULL){
      struct sockaddr_in addr;
      socklen_t len = sizeof(struct sockaddr_in);
      int ret;
      char tmp[4096] = {0};
      ret = recvfrom(_sockfd, tmp, 4096, 0, (struct sockaddr*)&addr, &len);
      if(ret < 0){
        perror("recvfrom erroe!\n");
        return false;
      }
      buf->assign(tmp, ret); // 给buf申请ret大小空间，从tmp中拷贝ret长度的数据进去
      if(ip != NULL){
        *ip = inet_ntoa(addr.sin_addr); // 将网络字节序整数IP地址转换为字符串地址
      }
      if(port != NULL){
        *port = ntohs(addr.sin_port);
      }
      return true;
    }
    // 5.关闭套接字
    void Close(){
      close(_sockfd);
      _sockfd = -1;
      return;
    }
  private:
      int _sockfd;
};

// 客户端要给服务端发送数据，那么就需要知道服务端的地址信息
// 因此通过程序运行参数传入服务端的地址信息
int main(int argc, char *argv[])
{
  if(argc != 3){
    printf("em : ./udp_cli 192.168.122.132 9000\n");
    return -1;
  }
  std::string ip_addr = argv[1];
  uint16_t port_addr = atoi(argv[2]);

  UdpSocket sock;
  sock.Socket();
  while(1){
    std::cout << "client say : ";
    std::string buf;
    std::cin >> buf;
    sock.Send(buf, ip_addr, port_addr);

    buf.clear();
    sock.Recv(&buf);
    std::cout << "server say : " << buf << std:: endl;
  }
  sock.Close();
  return 0;
}
