#include <cstring>
extern "C" {
#include "unp.h"
}
#include <cstdio>
#include <strings.h>
#include <sys/socket.h>

void my_str_echo(int sockfd) {
  ssize_t n;
  char buf[MAXLINE];

again:
  while ((n = read(sockfd, buf, MAXLINE)) > 0) {
    // memset(buf, 0, sizeof(buf));
    Writen(sockfd, buf, n);

    // 从缓冲器中读出数据到终端
    // fflush(stdout);
    // Fputs(buf, stdout);
    printf("%s", buf);
    memset(buf, 0, sizeof(buf));
  }

  // 错误处理
  if (n < 0 && errno == EINTR)
    goto again;
  else if (n < 0)
    err_sys("str_echo: read error");
}

// using namespace std;
int main(int argc, char *argv[]) {
  int listenfd, connfd;
  pid_t childpid;
  socklen_t clilen;
  struct sockaddr_in cliaddr, servaddr;
  // void sig_chld(int);

  listenfd = Socket(AF_INET, SOCK_STREAM, 0);
  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(SERV_PORT);

  Bind(listenfd, (SA *)&servaddr, sizeof(servaddr));

  // 开始监听客户端的连接请求
  listen(listenfd, LISTENQ);

  // Signal(SIGCHLD, sig_chld); // POSIX 信号处理

  for (;;) {
    clilen = sizeof(cliaddr);

    // 服务器阻塞于 accept 调用，等待客户连接的完成
    connfd = Accept(listenfd, (SA *)&cliaddr, &clilen);

    if ((childpid = Fork()) == 0) {
      Close(listenfd);
      my_str_echo(connfd);
      exit(0);
    }
    Close(0);
  }
}