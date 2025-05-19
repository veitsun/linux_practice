#include <iostream>

#include <thread>
// using namespace std;

int main() {
  std::cout << std::thread::hardware_concurrency() << std::endl;
  // 在多核系统上，可能是CPU的核心数量

  return 0;
}