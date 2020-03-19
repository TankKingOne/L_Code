#include <iostream>
#include <stdlib.h>
#include <signal.h>
#include <sys/wait.h>
#include "tcpsocket.hpp"

void sigcb(int signo){
  // 当子进程退出的时候就会像父进程发送SIGCHLD信号，回调这个函数
  // waitpid返回值 >0 表示处理了一个退出的子进程
  // waitpid <= 0 表示没有退出的子进程了
  while(waitpid(-1, 0, WNOHANG) > 0){}; // 一次循环将退出的子进程全部处理
}

int main(int argc, char *argv[])
{
  if(argc != 3){
    printf("em : ./tcp_srv 127.0.0.1 9000\n");
    return -1;
  }
  std::string ip = argv[1];
  uint16_t port = atoi(argv[2]);

  signal(SIGCHLD, sigcb);
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
    printf("client : [%s : %d]\n", &cli_ip[0], cli_port);
    /******************************************/
    pid_t pid = fork();
    if(pid == 0){ // 子进程复制父进程----数据独有，代码共享
      // 让子进程与客户端通信
      while(1){
        std::string buf;
        if(cli_sock.Recv(&buf) == false){
          cli_sock.Close();
          continue;
        }
        printf("client : [%s : %d] say : %s\n", &cli_ip[0], cli_port, &buf[0]);

        std::cout << "server say : ";
        fflush(stdout);
        buf.clear();
        std::cin >> buf;

        if(cli_sock.Send(buf) == false){
          cli_sock.Close();
          continue;
        }
      }
      cli_sock.Close();
      exit(0);
    }
    cli_sock.Close(); // 关闭父进程的文件描述符
  }
  lst_sock.Close();
  return 0;
}
