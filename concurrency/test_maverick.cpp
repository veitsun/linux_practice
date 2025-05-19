#include "Counter.h"
#include "test.h"
#include <functional>
#include <iostream>
#include <thread>
void do_some() { std::cout << "do some thing" << std::endl; }

struct test_sunwei {
  void show() { std::cout << "Hello, World hhhh!" << std::endl; }
};

int main() {
  test_sunwei t;
  t.show();
  return 0;
}