#include <iostream>
#include <thread>

// 线程数组

void func(int &num) { num++; }

int main(int argc, char *argv[]) {
  int num = 0;
  std::thread threads[5];
  for (int i = 0; i < 5; i++) {
    threads[i] = std::thread(func, std::ref(num));
  }

  // join
  for (auto &t : threads) {
    t.join();
  }
  //
  std::cout << num << std::endl;
  std::cout << "主线程退出" << std::endl;
  return 0;
}