#include "Counter.h"
#include "test.h"
#include <iostream>

int main() {
  test t;
  Counter c;
  c();
  c();
  c();
  std::cout << "Counter :" << c.get() << std::endl;
  t.show();
  return 0;
}