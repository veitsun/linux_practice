#include "unp.h"

/**
 * @brief
 *
 * @param fp stdin
 * 是标准输入流，他是一个文件指针，指向程序的输入源，通常是用户从键盘输入的内容，预定义的文件流，通常用于读取用户的输入
 * @param sockfd
 */
void str_cli(FILE *fp, int sockfd) {
  char sendline[MAXLINE], recvline[MAXLINE];

  while (Fgets(sendline, MAXLINE, fp) != NULL) {

    // 通过 sockfd 套接字将 sendline 中的内容发送到连接的远程服务器
    Writen(sockfd, sendline, strlen(sendline));

    // Readline
    // 是一个封装函数，通常用于从套接字中逐行读取数据。他会读取数据直到遇到换行符，或者达到最大读取的字节数
    // MAXLINE if 是 0 ，表示对方关闭了连接，即受到文件结束符 EOF
    if (Readline(sockfd, recvline, MAXLINE) == 0)
      err_quit("str_cli: server terminated prematurely");

    Fputs(recvline, stdout);
  }
}
