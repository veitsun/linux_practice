#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

int main() {
  printf("1");
  // int sockfd;
  // struct sockaddr_in server_addr;
  // char send_data[] = "Hello, Server!";
  // char recv_data[1024] = {0};

  // // 创建套接字
  // sockfd = socket(AF_INET, SOCK_STREAM, 0);

  // // 配置服务器地址
  // server_addr.sin_family = AF_INET;
  // server_addr.sin_port = htons(8080);
  // inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr);

  // // 连接服务器
  // if (connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) <
  //     0) {
  //   perror("Connection failed??");
  //   close(sockfd);
  //   return 1;
  // }

  // // 发送数据
  // send(sockfd, send_data, strlen(send_data), 0);
  // printf("Data sent: %s\n", send_data);

  // // 接收数据
  // recv(sockfd, recv_data, sizeof(recv_data), 0);
  // printf("Data received: %s\n", recv_data);

  // // 关闭套接字
  // close(sockfd);
  return 0;
}
