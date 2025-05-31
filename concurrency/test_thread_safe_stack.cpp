#include "ThreadSafeStack.hpp"
#include <atomic>
#include <cassert>
#include <chrono>
#include <iostream>
#include <random>
#include <thread>
#include <vector>

// std::make_unique 是 c14 的标准，我的项目是使用c11，需要自己实现一下
#if __cplusplus <= 201103L
namespace std {
template <typename T, typename... Args>
std::unique_ptr<T> make_unique(Args &&...args) {
  return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}
} // namespace std
#endif

// 测试辅助函数
void test_basic_functionality() {
  std::cout << "测试基本功能..." << std::endl;

  ThreadSafeStack<int> stack;

  // 测试入栈
  stack.push(1);
  stack.push(2);
  stack.push(3);

  // 测试出栈
  auto value1 = stack.pop();
  assert(*value1 == 3);
  std::cout << "弹出: " << *value1 << std::endl;

  auto value2 = stack.pop();
  assert(*value2 == 2);
  std::cout << "弹出: " << *value2 << std::endl;

  auto value3 = stack.pop();
  assert(*value3 == 1);
  std::cout << "弹出: " << *value3 << std::endl;

  std::cout << "基本功能测试通过!" << std::endl;
}

void test_empty_stack_exception() {
  std::cout << "\n测试空栈异常..." << std::endl;

  ThreadSafeStack<int> stack;

  try {
    stack.pop();
    assert(false); // 不应该到达这里
  } catch (const empty_stack &e) {
    std::cout << "捕获到预期异常: " << e.what() << std::endl;
  }

  std::cout << "空栈异常测试通过!" << std::endl;
}

void test_different_types() {
  std::cout << "\n测试不同数据类型..." << std::endl;

  // 测试字符串
  ThreadSafeStack<std::string> string_stack;
  string_stack.push("Hello");
  string_stack.push("World");

  auto str1 = string_stack.pop();
  assert(*str1 == "World");
  std::cout << "字符串弹出: " << *str1 << std::endl;

  // 测试浮点数
  ThreadSafeStack<double> double_stack;
  double_stack.push(3.14);
  double_stack.push(2.71);

  auto d1 = double_stack.pop();
  assert(std::abs(*d1 - 2.71) < 0.001);
  std::cout << "浮点数弹出: " << *d1 << std::endl;

  std::cout << "不同数据类型测试通过!" << std::endl;
}

void test_multithreaded_push_pop() {
  std::cout << "\n测试多线程推入弹出..." << std::endl;

  ThreadSafeStack<int> stack;
  const int num_threads = 4;
  const int items_per_thread = 1000;
  std::atomic<int> total_popped(0);
  std::vector<std::thread> threads;

  // 生产者线程
  for (int i = 0; i < num_threads; ++i) {
    threads.emplace_back([&stack, i, items_per_thread]() {
      for (int j = 0; j < items_per_thread; ++j) {
        stack.push(i * items_per_thread + j);
      }
    });
  }

  // 消费者线程
  for (int i = 0; i < num_threads; ++i) {
    threads.emplace_back([&stack, &total_popped, items_per_thread]() {
      for (int j = 0; j < items_per_thread; ++j) {
        bool success = false;
        while (!success) {
          try {
            auto value = stack.pop();
            total_popped++;
            success = true;
          } catch (const empty_stack &) {
            // 栈为空，稍等后重试
            std::this_thread::sleep_for(std::chrono::microseconds(1));
          }
        }
      }
    });
  }

  // 等待所有线程完成
  for (auto &t : threads) {
    t.join();
  }

  assert(total_popped == num_threads * items_per_thread);
  std::cout << "多线程测试通过! 总共处理了 " << total_popped << " 个元素"
            << std::endl;
}

void test_concurrent_mixed_operations() {
  std::cout << "\n测试并发混合操作..." << std::endl;

  ThreadSafeStack<int> stack;
  std::atomic<bool> should_stop(false);
  std::atomic<int> push_count(0);
  std::atomic<int> pop_count(0);
  std::vector<std::thread> threads;

  // 随机推入线程
  threads.emplace_back([&]() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 100);

    for (int i = 0; i < 500; ++i) {
      stack.push(dis(gen));
      push_count++;
      std::this_thread::sleep_for(std::chrono::microseconds(10));
    }
  });

  // 随机弹出线程
  threads.emplace_back([&]() {
    while (pop_count < 500) {
      try {
        auto value = stack.pop();
        pop_count++;
      } catch (const empty_stack &) {
        std::this_thread::sleep_for(std::chrono::microseconds(10));
      }
    }
  });

  // 等待线程完成
  for (auto &t : threads) {
    t.join();
  }

  assert(push_count == 500);
  assert(pop_count == 500);
  std::cout << "并发混合操作测试通过! 推入: " << push_count
            << ", 弹出: " << pop_count << std::endl;
}

void test_stress_test() {
  std::cout << "\n进行压力测试..." << std::endl;

  ThreadSafeStack<int> stack;
  const int stress_iterations = 10000;
  std::vector<std::thread> threads;

  auto start_time = std::chrono::high_resolution_clock::now();

  // 多个生产者
  for (int t = 0; t < 3; ++t) {
    threads.emplace_back([&stack, t, stress_iterations]() {
      for (int i = 0; i < stress_iterations; ++i) {
        stack.push(t * stress_iterations + i);
      }
    });
  }

  // 多个消费者
  std::atomic<int> consumed(0);
  for (int t = 0; t < 3; ++t) {
    threads.emplace_back([&stack, &consumed, stress_iterations]() {
      for (int i = 0; i < stress_iterations; ++i) {
        while (true) {
          try {
            auto value = stack.pop();
            consumed++;
            break;
          } catch (const empty_stack &) {
            std::this_thread::yield();
          }
        }
      }
    });
  }

  for (auto &t : threads) {
    t.join();
  }

  auto end_time = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(
      end_time - start_time);

  assert(consumed == 3 * stress_iterations);
  std::cout << "压力测试通过! 处理了 " << consumed << " 个元素，用时 "
            << duration.count() << " 毫秒" << std::endl;
}

void test_memory_safety() {
  std::cout << "\n测试内存安全..." << std::endl;

  ThreadSafeStack<std::unique_ptr<int>> stack;

  // 推入智能指针
  stack.push(std::make_unique<int>(42));
  stack.push(std::make_unique<int>(24));

  // 弹出并验证
  auto ptr1 = stack.pop();
  assert(**ptr1 == 24);
  std::cout << "智能指针值: " << **ptr1 << std::endl;

  auto ptr2 = stack.pop();
  assert(**ptr2 == 42);
  std::cout << "智能指针值: " << **ptr2 << std::endl;

  std::cout << "内存安全测试通过!" << std::endl;
}

int main() {
  std::cout << "开始 ThreadSafeStack 测试...\n" << std::endl;

  try {
    test_basic_functionality();
    test_empty_stack_exception();
    test_different_types();
    test_multithreaded_push_pop();
    test_concurrent_mixed_operations();
    test_stress_test();
    test_memory_safety();

    std::cout << "\n🎉 所有测试通过!" << std::endl;

  } catch (const std::exception &e) {
    std::cerr << "测试失败: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}