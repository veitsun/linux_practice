#include <chrono>
#include <functional>
#include <iostream>
#include <thread>
#include <utility>

using namespace std;

void func1(int n) {
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
  std::cout << n << std::endl;
}
void func2(int &n) {
  n += 1;
  std::cout << "执行了func2" << std::endl;
}

int main() {
  int n = 0;
  std::thread t1;
  std::thread t2(func1, 5);
  std::thread t3(func2, std::ref(n));
  std::thread t4(std::move(t3));
  t2.join();
  // t3.join();
  t4.join();
  // std::cout << "final vaule of n is " << n <<
  std::cout << "最终的n是" << n << std::endl;
  std::cout << "主线程退出" << std::endl;
  return 0;
}