#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>

int main() {
  const char *path = "example.dat";
  int fd = open(path, O_RDWR | O_CREAT, 0666);
  ftruncate(fd, 4096);  // 文件至少 4KB

  // 映射 4KB，读写，共享
  void *addr = mmap(NULL, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
  if (addr == MAP_FAILED) {
    perror("mmap");
    return 1;
  }

  // 在映射区写入数据
  strcpy((char *)addr, "Hello, mmap!");

  // 强制写回磁盘（可选）
  if (msync(addr, 4096, MS_SYNC) < 0) {
    perror("msync");
  }

  // 解除映射并关闭文件
  munmap(addr, 4096);
  close(fd);
  return 0;
}
