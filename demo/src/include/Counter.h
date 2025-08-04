// 这里有一个计数器的例子，可以记录自己被调用多少次
#ifndef COUNTER_H
#define COUNTER_H
#include <iostream>
class Counter {
private:
  int count;

public:
  Counter() : count(0) {}

  void operator()();
  int get() const;
};
#endif

/*


#include "Counter.h"
#include "test.h"
#include <functional>
#include <iostream>
#include <thread>
void do_some() { std::cout << "do some thing" << std::endl; }

int main() {
  // Counter c;
  Counter c2;
  std::thread t1([]() {
    Counter c;
    for (int i = 0; i < 1000; ++i) {
      c();
    }
    std::cout << "Thread 1 finished" << std::endl;
    std::cout << "Counter value: " << c.get() << std::endl;
  });
  t1.join();

  std::thread t2(std::ref(c2));
  t2.join();
  std::cout << "Thread 2 finished" << std::endl;
  std::cout << "Counter value: " << c2.get() << std::endl;

  std::thread t3(do_some);
  t3.join();
  std::cout << "Thread 3 finished" << std::endl;

  return 0;
}




*/
