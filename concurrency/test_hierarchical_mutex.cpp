#include "HierarchicalMutex.hpp"
#include <iostream>

Hierarchical_mutex mutex1(10000);
Hierarchical_mutex mutex2(2000);
Hierarchical_mutex mutex3(1000);
Hierarchical_mutex mutex4(100);

int main() {
  mutex1.lock();
  mutex2.lock();
  mutex3.lock();
  mutex4.lock();
  std::cout << "All mutexes locked successfully." << std::endl;

  mutex4.unlock();
  mutex3.unlock();
  mutex2.unlock();
  mutex1.unlock();
  std::cout << "All mutexes unlocked successfully." << std::endl;

  std::cout << "hierarchy value: "
            << Hierarchical_mutex::get_thread_hierarchy_value() << std::endl;
  return 0;
}