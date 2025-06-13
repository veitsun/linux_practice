// std::promise 允许在一个线程中设置值，在另一个线程中通过 std::future 获取值

#include <chrono>
#include <future>
#include <iostream>
#include <string>
#include <thread>
// std::promise 和 std::future 是 C++11 中用于线程间通信的工具

void worker_thread(std::promise<std::string> &&promise) {
  std::this_thread::sleep_for(std::chrono::seconds(2)); // 模拟耗时操作
  promise.set_value("Hello from worker thread!");       // 设置值
}

void worker_with_exception(std::promise<int> &&promise) {
  try {
    std::this_thread::sleep_for(std::chrono::seconds(1));
    throw std::runtime_error("An error occurred in worker thread!");
  } catch (const std::exception &e) {
    promise.set_exception(std::current_exception()); // 设置异常
  }
}

int main() {
  // 正常情况
  std::promise<std::string> promise1;
  // 从 promise 对象中获取与之配对的 future 对象，建立他们之间的连接通道
  // 返回一个与该 promise 绑定的 std::future<std::string> 对象
  // 这个 future 对象可以用来接受 promise 将来设置的值
  // 如果没有 future 对象，就没有办法接受 worker
  // 线程设置的值，整个异步通信就无法完成
  std::future<std::string> future1 = promise1.get_future();

  std::thread t1(worker_thread, std::move(promise1));

  std::cout << "等待 worker thread 结果..." << std::endl;
  std::string result = future1.get();
  std::cout << "收到结果: " << result << std::endl;

  t1.join();

  // 示例2: 异常情况
  std::promise<int> promise2;
  std::future<int> future2 = promise2.get_future();

  std::thread t2(worker_with_exception, std::move(promise2));

  try {
    int value = future2.get();
    std::cout << "结果: " << value << std::endl;
  } catch (const std::exception &e) {
    std::cout << "捕获异常: " << e.what() << std::endl;
  }

  t2.join();

  return 0;
}