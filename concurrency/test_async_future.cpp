#include <chrono>
#include <future>
#include <iostream>
#include <thread>

int heavy_computation(int n) {
  std::this_thread::sleep_for(std::chrono::seconds(10));
  return n * n;
  // 这里模拟了一个很耗时的操作
}

int main() {
  std::cout << "启动异步任务..." << std::endl;

  std::future<int> result =
      std::async(std::launch::async, heavy_computation, 10);

  std::cout << "异步任务已启动，主线程可以继续执行其他操作。" << std::endl;
  // 任务正在后台运行，可以做其他事情

  // 做一些其他事情
  for (int i = 0; i < 5; i++) {

    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::cout << "主线程时间已经过去一秒" << std::endl;
  }

  std::cout << "等待异步任务完成..." << std::endl;
  // 获取异步线程任务的结果，（会阻塞直到任务完成）

  int value = result.get(); // 没有拿到结果会阻塞在这里
  std::cout << "异步任务完成，结果是: " << value << std::endl;

  std::cout << "主线程结束。" << std::endl;

  return 0;
}