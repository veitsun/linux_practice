#include "CTest.h"
#include <cstdio>
#include <iostream>

using namespace std;

int main() {
  int n;
  cin >> n;
  cout << n << " hh " << endl;

  CTest c;
  c.set(1);
  cout << c.get() << endl;
  return 0;
}