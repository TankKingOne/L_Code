#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h> // sockaddr结构体 / IPPROTO_UDP
#include <arpa/inet.h> // 包含一些字节序转换的接口
#include <sys/socket.h> // 套接字接口头文件

int main(int argc, char *argv[])
{
  // argc表示参数个数，通过argv向程序传递端口参数
  if(argc != 3){
    printf("./udp_srv ip port em: ./udp_srv 192.168.122.132 9000\n");
    return -1;
  }
  const char *ip_addr = argv[1];
  uint16_t port_addr = atoi(argv[2]);

  // socket(地址域, 套接字类型, 协议类型);
  int sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if(sockfd < 0){
    perror("socket error!\n");
    return -1;
  }

  // bind(套接字描述符, 地址结构, 地址长度);
  // struct sockaddr_in IPv4地址结构
  // struct in_addr{ uint32_t s_addr }
  struct sockaddr_in addr;
  addr.sin_family = AF_INET;
  addr.sin_port = htons(port_addr);
  // inet_addr 将一个点分十进制的字符串IP地址转换为网络字节序的整数IP地址
  addr.sin_addr.s_addr = inet_addr(ip_addr);
  socklen_t len = sizeof(struct sockaddr_in); // 获取IPv4地址结构长度
  int ret = bind(sockfd, (struct sockaddr*)&addr, len);
  if(ret < 0){
    perror("bind error!\n");
    return -1;
  }

  while(1){
    char buf[1024] = {0};
    struct sockaddr_in cli_addr;
    socklen_t len = sizeof(struct sockaddr_in);
    // recvfrom(描述符, 缓冲区, 长度, 参数, 客户端地址信息, 地址信息长度);
    // 阻塞接收数据，将数据放入buf中，将发送端的地址放入cli_addr中
    int ret = recvfrom(sockfd, buf, 1023, 0, (struct sockaddr*)&cli_addr, &len);
    if(ret < 0){
      perror("recvfrom error!\n");
      return -1;
    }
    printf("clicent say : %s\n", buf);
    printf("server say : ");
    fflush(stdout);
    memset(buf, 0x00, 1024); // 清空buf中的数据
    scanf("%s", buf);

    sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr*)&cli_addr, len);
    
    if(ret < 0){
      perror("sendto error!\n");
      close(sockfd);
      return -1;
    }
  }
}
