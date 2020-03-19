#include <iostream>
#include <stdlib.h>
#include <pthread.h>
#include "tcpsocket.hpp"

void *thr_start(void *arg){
  long fd = (long)arg;
  TcpSocket cli_sock;
  cli_sock.SetFd(fd);
  while(1){
    std::string buf;
    if(cli_sock.Recv(&buf) == false){
      cli_sock.Close();
      pthread_exit(NULL);
      continue;
    }
    printf("client say : %s\n", &buf[0]);

    std::cout << "server say : ";
    fflush(stdout);
    buf.clear();
    std::cin >> buf;

    if(cli_sock.Send(buf) == false){
      cli_sock.Close();
      pthread_exit(NULL);
    }
  }
  cli_sock.Close();
  return NULL;
}

int main(int argc, char *argv[])
{
  if(argc != 3){
    printf("em : ./tcp_srv 127.0.0.1 9000\n");
    return -1;
  }
  std::string ip = argv[1];
  uint16_t port = atoi(argv[2]);

  TcpSocket lst_sock;
  CHECK_RET(lst_sock.Socket());
  CHECK_RET(lst_sock.Bind(ip, port));
  CHECK_RET(lst_sock.Listen());
  while(1){
    TcpSocket cli_sock;
    std::string cli_ip;
    uint16_t cli_port;

    bool ret = lst_sock.Accept(&cli_sock, &cli_ip, &cli_port);
    if(ret == false){
      continue;
    }

    printf("client : [%s : %d]", &cli_ip[0], cli_port);
    //---------------------------------------------
    pthread_t tid;
    // cli_sock是一个局部变量--循环完了这个资源就会被释放
    pthread_create(&tid, NULL, thr_start, (void*)cli_sock.GetFd());
    pthread_detach(tid); // 不关心线程返回值，分离线程，退出后自动释放资源
    // 主线程不能关闭cli_sock，因为多线程是共用描述符的
  }
  lst_sock.Close();
  return 0;
}
