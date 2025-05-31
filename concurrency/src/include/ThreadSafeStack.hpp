#pragma once
#include <exception>
#include <memory>
#include <mutex>
#include <stdexcept>
#include <utility>
#include <vector>

struct empty_stack : std::exception {
  const char *what() const noexcept override {
    // 重写 what() 函数，返回异常信息
    return "ThreadSafeStack<>::pop(): empty stack";
  }
};

// 定义一个线程安全栈模板类，使用类型参数 T
template <typename T> class ThreadSafeStack {
public:
  ThreadSafeStack() = default;

  // 禁用拷贝构造函数和拷贝赋值运算符

  ThreadSafeStack &operator=(const ThreadSafeStack &) = delete;
  ThreadSafeStack(const ThreadSafeStack &) = delete;

  // void push(const T &value) {
  //   std::lock_guard<std::mutex> lock(mutex_);
  //   stack_.push_back(value);
  // }

  // 入栈操作
  void push(T &&value) {
    std::lock_guard<std::mutex> lock(mutex_);
    stack_.push_back(std::move(value));
  }

  // 出栈操作
  std::shared_ptr<T> pop() {
    std::lock_guard<std::mutex> lock(mutex_);
    if (stack_.empty()) {
      throw empty_stack();
    }
    std::shared_ptr<T> value = std::make_shared<T>(std::move(stack_.back()));
    stack_.pop_back();
    return value;
  }

private:
  std::vector<T> stack_;
  mutable std::mutex mutex_;
};
