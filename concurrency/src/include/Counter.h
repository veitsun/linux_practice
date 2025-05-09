// 这里有一个计数器的例子，可以记录自己被调用多少次
#ifndef COUNTER_H
#define COUNTER_H
#include <iostream>
class Counter {
private:
  int count;

public:
  Counter() : count(0) {}

  int operator()();
  int get() const;
};
#endif
