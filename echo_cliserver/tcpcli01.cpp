extern "C" {
#include "unp.h"
}
#include <cstdio>
#include <cstdlib>
#include <strings.h>
#include <sys/socket.h>

int main(int argc, char *argv[]) {
  int sockfd;
  struct sockaddr_in servaddr;

  if (argc != 2) {
    err_quit("usage : tcpcli <IPaddress>");
  }

  // 创建一个 IPv4 的字节流类型的套接字
  sockfd = Socket(AF_INET, SOCK_STREAM, 0);

  // 将 servaddr 结构体中的所有字节都设置为 0
  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_port = htons(SERV_PORT);

  // 将一个点分十进制的 IP
  // 地址，转换成网络字节序的二进制地址
  // 并将结果存储到 seraddr.sin_addr 中
  // AF_INET 表示 IPv4 地址族
  // argv[1] 是一个 ip 地址字符串
  Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

  // 通过套接字 sockfd 与指定的服务器地址 servaddr 建立连接
  Connect(sockfd, (SA *)&servaddr, sizeof(servaddr));

  str_cli(stdin, sockfd);

  exit(0);
}