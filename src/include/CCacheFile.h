#ifndef CCACHEFILE_H
#define CCACHEFILE_H

#include <cstddef>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

// 单缓冲策略
class CCacheFile {
private:
  FILE *file;
  std::ofstream output;     // 输出流
  std::ifstream input;      // 输入流
  std::vector<char> buffer; // 缓冲区
  size_t buffer_size;       // 缓冲区大小
  size_t buffer_position;   // 当前缓冲区指针所在位置
  std::string filename;     // 文件名字

public:
  CCacheFile();
  ~CCacheFile();
  bool open(const char *name);
  size_t read(size_t pos, size_t length);
  size_t write(size_t pos, const char *src);
};

#endif