#include <chrono>
#include <future>
#include <iostream>
#include <thread>
#include <vector>

// 计算阶乘的函数
long long factorial(int n) {
  long long result = 1;
  for (int i = 1; i <= n; ++i) {
    result *= i;
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
  }
  return result;
}

int main() {
  // 创建 packaged_task
  // long long factorial(int n) 是一个计算阶乘的函数
  // std::packaged_task<long long(int)> 表示包装一个 “接受int参数，返回 long
  // long” 的函数
  std::packaged_task<long long(int)> task(factorial);

  // 获取与任务关联的 future
  std::future<long long> result = task.get_future();

  // 在新线程中执行任务
  // 将 task 移动到新线程，并传入参数 10
  std::thread t(std::move(task), 10);

  std::cout << "计算 10! 中..." << std::endl;

  // 等待结果
  long long value = result.get();
  std::cout << "10! = " << value << std::endl;

  t.join();

  // 示例：使用线程池模式
  std::cout << "\n=== 线程池示例 ===" << std::endl;

  // 上一个例子只是一个 future 的例子， 这个结构用来存储所有 future
  std::vector<std::future<long long>> futures;
  std::vector<std::thread> threads;

  // 创建多个任务
  for (int i = 5; i <= 8; ++i) {
    std::packaged_task<long long(int)> pool_task(factorial);
    futures.push_back(pool_task.get_future());
    // 将任务移动到线程中执行
    threads.emplace_back(std::move(pool_task), i);
  }

  // 获取所有结果
  for (size_t i = 0; i < futures.size(); ++i) {
    long long result = futures[i].get();
    std::cout << (i + 5) << "! = " << result << std::endl;
  }

  // 等待所有线程完成
  for (auto &t : threads) {
    t.join();
  }

  return 0;
}