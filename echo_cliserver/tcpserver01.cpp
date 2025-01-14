extern "C" {
#include "unp.h"
}
#include <cstdio>
#include <strings.h>
#include <sys/socket.h>

// using namespace std;
int main(int argc, char *argv[]) {
  int listenfd, connfd;
  pid_t childpid;
  socklen_t clilen;
  struct sockaddr_in cliaddr, servaddr;

  listenfd = Socket(AF_INET, SOCK_STREAM, 0);
  bzero(&servaddr, sizeof(servaddr));
  servaddr.sin_family = AF_INET;
  servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
  servaddr.sin_port = htons(SERV_PORT);

  Bind(listenfd, (SA *)&servaddr, sizeof(servaddr));
  listen(listenfd, LISTENQ);
  // 服务器阻塞于 accept 调用，等待客户连接的完成

  for (;;) {
    clilen = sizeof(cliaddr);
    connfd = Accept(listenfd, (SA *)&cliaddr, &clilen);

    if ((childpid = Fork()) == 0) {
      Close(listenfd);
      str_echo(connfd);
      exit(0);
    }
    Close(0);
  }
}