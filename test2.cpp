// ls -l
// 获取目录下的所有文件

#include <cstddef>
#include <cstdio>
#include <cstring>
#include <ctime>
#include <dirent.h>
#include <iomanip>
#include <iostream>
#include <sys/stat.h>

int main(int argc, char *argv[]) {
  // std::cout << "1" << std::endl;
  if (argc != 2) {
    std::cout << "error" << std::endl;
    return -1;
  }
  if (strcmp(argv[1], "-l") != 0) {
    std::cout << "error2" << std::endl;
    return -1;
  }

  DIR *d;
  d = opendir("./");
  if (d == nullptr) {
    std::cout << "打开文件失败" << std::endl;
  }
  struct dirent *entry;
  char pathname[256];
  while ((entry = readdir(d)) != nullptr) {
    std::cout << std::left << std::setw(20) << entry->d_name;

    size_t sz = strlen(entry->d_name);
    sprintf(pathname, "./%s", entry->d_name);
    // FILE *file = fopen(buffer, "r");
    struct stat buffer;
    if (!stat(pathname, &buffer)) {
      // 检查文件类型
      if (S_ISREG(buffer.st_mode)) {
        std::cout << std::setw(25) << "Type: Regular file";
      } else if (S_ISDIR(buffer.st_mode)) {
        std::cout << std::setw(25) << "Type: Directory";
      } else if (S_ISCHR(buffer.st_mode)) {
        std::cout << std::setw(25) << "Type: Character device";
      } else if (S_ISBLK(buffer.st_mode)) {
        std::cout << std::setw(25) << "Type: Block device";
      } else if (S_ISFIFO(buffer.st_mode)) {
        std::cout << std::setw(25) << "Type: FIFO (named pipe)";
      } else if (S_ISLNK(buffer.st_mode)) {
        std::cout << std::setw(25) << "Type: Symbolic link";
      } else if (S_ISSOCK(buffer.st_mode)) {
        std::cout << std::setw(25) << "Type: Socket";
      } else {
        std::cout << std::setw(25) << "Type: Unknown";
      }
      std::cout << "File size:" << buffer.st_size << std::setw(20) << "bytes"
                << "\t";
      std::cout << "File permissions:" << std::oct << std::setw(20)
                << (buffer.st_mode & 0777) << std::dec;
      std::cout << "Last modified:" << std::setw(20) << ctime(&buffer.st_mtime);
    } else {
      std::cout << "error" << std::endl;
      return -1;
    }

    // std::cout << entry->d_ino << " ";
    // std::cout << entry->d_off << " ";
    // std::cout << entry->d_type << " ";
    // std::cout << entry->d_off;
    std::cout << std::endl;
  }

  closedir(d);

  return 0;
}