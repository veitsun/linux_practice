/**
 * @file HierarchicalMutex.hpp
 * @author wei sun (you@domain.com)
 * @brief HierarchicalMutex class for managing mutexes with hierarchical locking
 * @version 0.1
 * @date 2025-06-04
 *
 * @copyright Copyright (c) 2025
 *
 */

#pragma once
#include <climits>
#include <mutex>

class Hierarchical_mutex {
private:
  std::mutex internal_mutex;
  unsigned long const hierarchy_value;
  unsigned long previous_hierarchy_value;
  static thread_local unsigned long this_thread_hierarchy_value;

  void check_for_hierarchy_violation() {
    if (this_thread_hierarchy_value <= hierarchy_value) {
      throw std::logic_error("mutex hierarchy violation");
    }
  }

  void update_hierarchy_value() {
    previous_hierarchy_value = this_thread_hierarchy_value;
    this_thread_hierarchy_value = hierarchy_value;
  }

public:
  explicit Hierarchical_mutex(unsigned long value)
      : hierarchy_value(value), previous_hierarchy_value(0) {}

  void lock() {
    check_for_hierarchy_violation();
    internal_mutex.lock();
    update_hierarchy_value();
  }

  void unlock() {
    this_thread_hierarchy_value = previous_hierarchy_value;
    internal_mutex.unlock();
  }

  bool try_lock() {
    check_for_hierarchy_violation();
    if (internal_mutex.try_lock()) {
      update_hierarchy_value();
      return true;
    }
    return false;
  }

  static unsigned long get_thread_hierarchy_value() {
    return this_thread_hierarchy_value;
  }

  ~Hierarchical_mutex() = default;
};

// Define thread_local static member
thread_local unsigned long
    Hierarchical_mutex::this_thread_hierarchy_value(ULONG_MAX);