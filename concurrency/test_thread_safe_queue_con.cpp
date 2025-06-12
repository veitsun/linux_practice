#include "ThreadSafeQueueConVar.hpp"
#include <atomic>
#include <cassert>
#include <chrono>
#include <iostream>
#include <thread>
#include <vector>

// 简单的测试框架
class TestRunner {
private:
  static int test_count;
  static int passed_count;

public:
  static void assert_true(bool condition, const std::string &message) {
    test_count++;
    if (condition) {
      passed_count++;
      std::cout << "[PASS] " << message << std::endl;
    } else {
      std::cout << "[FAIL] " << message << std::endl;
    }
  }

  static void print_summary() {
    std::cout << "\n=== Test Summary ===" << std::endl;
    std::cout << "Total tests: " << test_count << std::endl;
    std::cout << "Passed: " << passed_count << std::endl;
    std::cout << "Failed: " << (test_count - passed_count) << std::endl;
    std::cout << "Success rate: " << (100.0 * passed_count / test_count) << "%"
              << std::endl;
  }
};

int TestRunner::test_count = 0;
int TestRunner::passed_count = 0;

// 基础功能测试
void test_basic_operations() {
  std::cout << "\n--- Testing Basic Operations ---" << std::endl;

  ThreadSafeQueue<int> queue;

  // 测试初始状态
  TestRunner::assert_true(queue.empty(), "New queue should be empty");

  // 测试push和try_pop
  queue.push(42);
  TestRunner::assert_true(!queue.empty(),
                          "Queue should not be empty after push");

  int value;
  bool success = queue.try_pop(value);
  TestRunner::assert_true(success,
                          "try_pop should succeed with available element");
  TestRunner::assert_true(value == 42, "Popped value should be 42");
  TestRunner::assert_true(queue.empty(), "Queue should be empty after pop");

  // 测试try_pop with shared_ptr
  queue.push(100);
  auto ptr = queue.try_pop();
  TestRunner::assert_true(ptr != nullptr,
                          "try_pop should return valid pointer");
  TestRunner::assert_true(*ptr == 100, "Shared_ptr should contain value 100");

  // 测试空队列的try_pop
  success = queue.try_pop(value);
  TestRunner::assert_true(!success, "try_pop should fail on empty queue");

  auto empty_ptr = queue.try_pop();
  TestRunner::assert_true(empty_ptr == nullptr,
                          "try_pop should return null ptr for empty queue");
}

// 测试阻塞操作
void test_blocking_operations() {
  std::cout << "\n--- Testing Blocking Operations ---" << std::endl;

  ThreadSafeQueue<std::string> queue;
  std::atomic<bool> thread_started(false);
  std::atomic<bool> value_received(false);
  std::string received_value;

  // 启动一个等待线程
  std::thread waiter(
      [&queue, &thread_started, &value_received, &received_value]() {
        std::cout << "你先等 10 s ,ok?" << std::endl;
        // std::this_thread::sleep_for(std::chrono::milliseconds(10000));
        std::cout << "辛苦你了，等待结束" << std::endl;
        thread_started = true;
        queue.wait_and_pop(received_value);
        std::cout << "线程收到值: " << received_value << std::endl;
        // std::this_thread::sleep_for(std::chrono::milliseconds(10000));

        value_received = true;
      });

  // 等待线程启动
  while (!thread_started) {
    std::cout << "等待线程启动..." << std::endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(2000));
  }
  std::cout << "你已经等完" << std::endl;
  // std::this_thread::sleep_for(std::chrono::milliseconds(10000));

  // 短暂延迟确保wait_and_pop已经在等待
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
  TestRunner::assert_true(!value_received, "Thread should be waiting");
  std::this_thread::sleep_for(std::chrono::milliseconds(5000));
  std::cout << "现在推送值，唤醒等待线程" << std::endl;

  // 推送值应该唤醒等待线程
  queue.push("Hello World");
  waiter.join();

  TestRunner::assert_true(value_received, "Thread should have received value");
  TestRunner::assert_true(received_value == "Hello World",
                          "Received value should be 'Hello World'");
  // std::this_thread::sleep_for(std::chrono::milliseconds(20000));
}

// 测试拷贝构造函数
void test_copy_constructor() {
  std::cout << "\n--- Testing Copy Constructor ---" << std::endl;

  ThreadSafeQueue<int> original;
  original.push(1);
  original.push(2);
  original.push(3);

  ThreadSafeQueue<int> copy(original);

  // 验证拷贝是独立的
  int val1, val2;
  bool success1 = original.try_pop(val1);
  bool success2 = copy.try_pop(val2);

  TestRunner::assert_true(success1 && success2,
                          "Both queues should have elements");
  TestRunner::assert_true(val1 == val2, "First elements should be equal");

  // 修改原始队列不应影响拷贝
  original.push(99);
  std::this_thread::sleep_for(std::chrono::milliseconds(1000));

  TestRunner::assert_true(original.try_pop(val1),
                          "Original should have new element");
  TestRunner::assert_true(val1 == 2, "Original next element should be 2");

  TestRunner::assert_true(copy.try_pop(val2),
                          "Copy should have second element");
  TestRunner::assert_true(val2 == 2, "Copy next element should be 2");

  // std::this_thread::sleep_for(std::chrono::milliseconds(200000));
}

// 多线程并发测试
void test_multithreaded_operations() {
  std::cout << "\n--- Testing Multithreaded Operations ---" << std::endl;

  ThreadSafeQueue<int> queue;
  const int num_producers = 4;
  const int num_consumers = 3;
  const int items_per_producer = 1000;

  std::atomic<int> total_produced(0);
  std::atomic<int> total_consumed(0);
  std::vector<int> consumed_values;
  std::mutex consumed_mutex;

  std::vector<std::thread> producers;
  std::vector<std::thread> consumers;

  // 启动生产者线程
  for (int i = 0; i < num_producers; ++i) {
    producers.emplace_back([&queue, &total_produced, i, items_per_producer]() {
      std::cout << "生产者线程 " << i << " 开始工作" << std::endl;
      for (int j = 0; j < items_per_producer; ++j) {
        int value = i * items_per_producer + j;
        queue.push(value);
        total_produced.fetch_add(1);
      }
      std::cout << "生产者线程 " << i << " 完成工作" << std::endl;
    });
  }

  // 启动消费者线程
  for (int i = 0; i < num_consumers; ++i) {
    consumers.emplace_back([&queue, &total_consumed, &consumed_values,
                            &consumed_mutex, num_producers, items_per_producer,
                            i]() {
      std::cout << "消费者线程 " << i << " 开始工作" << std::endl;
      while (total_consumed.load() < num_producers * items_per_producer) {
        if (total_consumed.load() % 1000 == 0) {
          std::cout << "消费者线程 " << i
                    << " 已消费: " << total_consumed.load() << " 个元素"
                    << std::endl;
        }
        int value;
        if (queue.try_pop(value)) {
          {
            std::lock_guard<std::mutex> lk(consumed_mutex);
            consumed_values.push_back(value);
          }
          total_consumed.fetch_add(1);
        } else {
          std::this_thread::sleep_for(std::chrono::microseconds(10000));
        }
      }
    });
  }

  // 等待所有线程完成
  for (auto &t : producers) {
    t.join();
  }
  for (auto &t : consumers) {
    t.join();
  }

  TestRunner::assert_true(total_produced.load() ==
                              num_producers * items_per_producer,
                          "All items should be produced");
  TestRunner::assert_true(total_consumed.load() ==
                              num_producers * items_per_producer,
                          "All items should be consumed");
  TestRunner::assert_true(queue.empty(),
                          "Queue should be empty after all operations");
  TestRunner::assert_true(consumed_values.size() ==
                              num_producers * items_per_producer,
                          "All values should be consumed");

  // std::this_thread::sleep_for(std::chrono::milliseconds(200000));
}

// 性能压力测试
void test_performance() {
  std::cout << "\n--- Testing Performance ---" << std::endl;

  ThreadSafeQueue<int> queue;
  std::vector<std::thread> threads;
  const int num_operations = 100;

  auto start = std::chrono::high_resolution_clock::now();
  // 单线程性能测试
  for (int i = 0; i < num_operations; ++i) {
    queue.push(i);
  }

  for (int i = 0; i < num_operations; ++i) {
    int value;
    queue.try_pop(value);
  }

  // duo线程性能测试
  for (int i = 0; i < num_operations; ++i) {
    threads.emplace_back([&queue, i]() {
      // queue.push(i);
      int value;
      queue.try_pop(value);
    });
  }

  for (int i = 0; i < num_operations; ++i) {
    threads.emplace_back([&queue, i]() {
      // int value;
      // queue.try_pop(value);
      queue.push(i);
    });
  }

  for (auto &t : threads) {
    t.join();
  }

  auto end = std::chrono::high_resolution_clock::now();
  auto duration =
      std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  // 为什么在中间增加了单线程测试部分，执行时间却变得更长了，
  // 这个现象很常见
  // 关键原因：缓存预热 + 执行模式优化
  // 直接开启多线程，CPU缓存未预热，代码路径陌生
  //

  // 100个try_pop线程启动，队列空 -> 大部分立即失败返回
  // 100个push线程启动 -> 可能与部分try_pop线程竞争
  // 线程生命周期重叠，调度复杂

  // 单线程预热：push 100 -> pop 100（队列又空了）
  // 100个try_pop线程启动，队列空 -> 立即失败，但是执行路径已优化
  // 100个push线程启动 -> 在更优化的环境中执行

  std::cout << "Duo-threaded " << num_operations
            << " push+pop operations took: " << duration.count() << "ms"
            << std::endl;
  TestRunner::assert_true(duration.count() < 5000,
                          "Operations should complete in reasonable time");

  // std::this_thread::sleep_for(std::chrono::milliseconds(200000));
}

// 边界条件测试
void test_edge_cases() {
  std::cout << "\n--- Testing Edge Cases ---" << std::endl;

  ThreadSafeQueue<int> queue;

  // 多次检查空队列
  for (int i = 0; i < 10; ++i) {
    TestRunner::assert_true(queue.empty(), "Queue should remain empty");
    int value;
    TestRunner::assert_true(!queue.try_pop(value),
                            "try_pop should fail on empty queue");
  }

  // 推送和立即弹出
  for (int i = 0; i < 100; ++i) {
    queue.push(i);
    int value;
    TestRunner::assert_true(queue.try_pop(value),
                            "Should be able to pop immediately");
    TestRunner::assert_true(value == i, "Value should match pushed value");
  }

  // 测试大量连续操作
  const int large_count = 10000;
  for (int i = 0; i < large_count; ++i) {
    queue.push(i);
  }

  TestRunner::assert_true(!queue.empty(),
                          "Queue should not be empty with many elements");

  for (int i = 0; i < large_count; ++i) {
    auto ptr = queue.try_pop();
    TestRunner::assert_true(ptr != nullptr,
                            "Should be able to pop all elements");
    TestRunner::assert_true(*ptr == i, "Elements should be in FIFO order");
  }

  TestRunner::assert_true(queue.empty(),
                          "Queue should be empty after popping all elements");
}

// 测试不同数据类型
void test_different_types() {
  std::cout << "\n--- Testing Different Data Types ---" << std::endl;

  // 测试字符串
  ThreadSafeQueue<std::string> string_queue;
  string_queue.push("Hello");
  string_queue.push("World");

  auto str_ptr = string_queue.wait_and_pop();
  TestRunner::assert_true(*str_ptr == "Hello",
                          "String queue should work correctly");

  // 测试自定义类型
  struct TestStruct {
    int id;
    std::string name;
    TestStruct(int i, const std::string &n) : id(i), name(n) {}
  };

  ThreadSafeQueue<TestStruct> struct_queue;
  struct_queue.push(TestStruct{1, "Test"});

  TestStruct result{0, ""};
  struct_queue.wait_and_pop(result);
  TestRunner::assert_true(result.id == 1 && result.name == "Test",
                          "Custom struct should work correctly");
}

int main() {
  std::cout << "Starting ThreadSafeQueue Tests..." << std::endl;

  try {
    test_basic_operations();
    test_blocking_operations();
    test_copy_constructor();
    test_multithreaded_operations();
    test_performance();
    test_edge_cases();
    test_different_types();

    TestRunner::print_summary();

  } catch (const std::exception &e) {
    std::cerr << "Test failed with exception: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}