#include <chrono>
#include <cstdint>
#include <cstdio>
#include <iostream>
#include <thread>

int main() {
  /*
   * 多线程完成矩阵和向量的乘法，并测试应用多线程和没有用到多线程的性能差异(线程数量可以自己设置)
   * 输入 矩阵的行数和列数，向量的长度
   */
  std::cout << "不可以直接查找答案,或者GPT完成" << std::endl;
  // 你电脑上的CPU核心数
  std::cout << "此电脑可并行的线程数量:" << std::thread::hardware_concurrency()
            << std::endl;
  int m = 0; // 矩阵行数
  int n = 0; // 矩阵列数
  std::cin >> m >> n;
  int p = n;                   // 向量长度
  int **matrix = new int *[m]; // 矩阵
  int *vector = new int[p];    // 向量
  int *result1 = new int[m];   // 结果1
  int *result2 = new int[m];   // 结果2

  // 矩阵数据随机生成
  for (int i = 0; i < m; ++i) {
    matrix[i] = new int[n];
    for (int j = 0; j < n; ++j) {
      matrix[i][j] = rand() % 100;
    }
  }
  // 向量数据随机生成
  for (int i = 0; i < p; ++i) {
    vector[i] = rand() % 100;
  }
  // 在我代码基础上，新增代码完成内容，不要删除我写的代码包括注释

  // for (int i = 0; i < m; ++i) {
  //   for (int j = 0; j < n; ++j) {
  //     std::cout << matrix[i][j] << " ";
  //   }
  //   if (i < p)
  //     std::cout << "        " << vector[i];
  //   std::cout << std::endl;
  // }
  // std::cout << "-------------" << std::endl;
  // for (int i = 0; i < p; ++i) {
  //   std::cout << vector[i] << " ";
  // }
  // std::cout << std::endl;
  // std::cout << "-------------" << std::endl;

  // 生成 m 个线程
  std::thread *threads = new std::thread[m];
  auto overall_start_time = std::chrono::high_resolution_clock::now();

  for (int i = 0; i < m; i++) {
    threads[i] = std::thread([=]() {
      int sum = 0;
      for (int j = 0; j < n; j++) {
        sum += matrix[i][j] * vector[j];
      }
      // std::cout << "Thread " << i << " finished, result: " << sum <<
      // std::endl;
      result1[i] = sum;
    });
  }

  for (int i = 0; i < m; i++) {
    threads[i].join();
  }
  auto overall_end_time = std::chrono::high_resolution_clock::now();
  auto duration_con = std::chrono::duration_cast<std::chrono::microseconds>(
                          overall_end_time - overall_start_time)
                          .count();

  delete[] threads;

  auto overall_start_time1 = std::chrono::high_resolution_clock::now();
  for (int i = 0; i < m; i++) {
    int sum = 0;
    for (int j = 0; j < n; j++) {
      sum += matrix[i][j] * vector[j];
    }
    result2[i] = sum;
  }
  auto overall_end_time1 = std::chrono::high_resolution_clock::now();
  auto duration_con1 = std::chrono::duration_cast<std::chrono::microseconds>(
                           overall_end_time1 - overall_start_time1)
                           .count();

  std::cout << "多线程计算耗时: " << duration_con << " 微秒" << std::endl;
  std::cout << "单线程计算耗时: " << duration_con1 << " 微秒" << std::endl;

  for (int i = 0; i < m; i++) {
    if (result1[i] != result2[i]) {
      std::cout << "Error: result1[" << i << "] = " << result1[i]
                << ", result2[" << i << "] = " << result2[i] << std::endl;
    }
  }
  return 0;
}