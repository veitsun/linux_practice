#include "CTest.h"
#include <iostream>

using namespace std;

int main() {
  CTest test;
  test.set(10);
  int return_int = test.get();
  std::cout << return_int << std::endl;
  return 0;
}