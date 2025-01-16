#include <asm-generic/socket.h>
#include <cctype>
#include <cstddef>
#include <cstdio>
#include <fcntl.h>
#include <netinet/in.h>
#include <pthread.h>
#include <strings.h>
#include <sys/socket.h>
extern "C" {
#include "unp.h"
}

// 线程处理函数
void *thread_handler(void *args) {
  char buf[BUFSIZ]; // BUFSIZ 内置宏 8192
  int n, i;
  intptr_t fd = (intptr_t)args; // 转换获得客户端描述符
  while (1) {
    bzero(buf, sizeof(buf));
    n = read(fd, buf, sizeof(buf));
    // 对方已关闭
    if (0 == n) {
      // 关闭连接
      close(fd);
      printf("close...\n");
      break;
    } else if (0 < n) {
      // 大小写转换
      for (i = 0; i < n; i++) {
        buf[i] = toupper(buf[i]);
      }
      // 发送给客户端
      write(fd, buf, n);
    }
  }
  pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
  pthread_t tid;
  int listenfd, connfd;
  struct sockaddr_in servaddr;
  struct sockaddr_in clienaddr;

  socklen_t clie_addr_len;
  int opt;
  char str[INET_ADDRSTRLEN]; // #define INET_ADDRSTRLEN 16

  // 创建监听套接字
  listenfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

  // 设置端口复用
  opt = 1;
  setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

  // 初始化为 0
  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(SERV_PORT);

  // 绑定到监听套接字
  Bind(listenfd, (struct sockaddr *)&servaddr, sizeof(servaddr));

  // 设置同时连接请求上限
  Listen(listenfd, SOMAXCONN);
  while (true) {
    clie_addr_len = sizeof(clienaddr);
    // 接受连接请求
    connfd = Accept(listenfd, (struct sockaddr *)&clienaddr, &clie_addr_len);
    // 打印客户端地址
    printf("connection from %s, port %d\n",
           inet_ntop(AF_INET, &clienaddr.sin_addr, str, sizeof(str)),
           ntohs(clienaddr.sin_port));

    pthread_create(&tid, NULL, thread_handler, (void *)&connfd);
    pthread_detach(tid);
    // 关闭连接套接字
    Close(connfd);
  }
}