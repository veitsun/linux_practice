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

    Writen(sockfd, sendline, strlen(sendline));

    if (Readline(sockfd, recvline, MAXLINE) == 0)
      err_quit("str_cli: server terminated prematurely");

    Fputs(recvline, stdout);
  }
}
