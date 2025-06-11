#include <iostream>
#include <mutex>
#include <thread>

std::mutex mtx;
std::mutex mtx2;
std::mutex mtx3;

void func1() {
  std::unique_lock<std::mutex> lock(mtx2, std::defer_lock);
  lock.lock();
  std::cout << "func1 acquired the lock." << std::endl;
  std::this_thread::sleep_for(std::chrono::seconds(2));
  lock.unlock();
  std::cout << "func1 released the lock." << std::endl;
}

void func2() {
  std::unique_lock<std::mutex> lock(mtx3, std::defer_lock);
  lock.lock();
  std::cout << "func2 acquired the lock." << std::endl;
  std::this_thread::sleep_for(std::chrono::seconds(2));
  lock.unlock();
  std::cout << "func2 released the lock." << std::endl;
}

int main() {
  std::unique_lock<std::mutex> lock(mtx, std::defer_lock);

  lock.lock();
  std::unique_lock<std::mutex> lock2(mtx, std::try_to_lock);
  if (lock2.owns_lock()) {
    std::cout << "lock2 acquired the lock." << std::endl;
  } else {
    std::cout << "lock2 could not acquire the lock." << std::endl;
  }
  lock.unlock();
  std::cout << "lock released the lock." << std::endl;
  // lock2.unlock();

  if (lock2.owns_lock()) {
    std::cout << "lock2 acquired the lock." << std::endl;
  } else {
    std::cout << "lock2 could not acquire the lock." << std::endl;
  }
  if (lock2.owns_lock()) {
    std::cout << "lock2 acquired the lock." << std::endl;
  } else {
    std::cout << "lock2 could not acquire the lock." << std::endl;
  }

  std::unique_lock<std::mutex> lock3(mtx, std::defer_lock);
  if (lock3.owns_lock()) {
    std::cout << "lock3 acquired the lock." << std::endl;
  } else {
    std::cout << "lock3 could not acquire the lock." << std::endl;
  }
  // 对 mtx 进行上锁操作
  // lock3.lock();
  // std::lock(mtx);
  std::lock_guard<std::mutex> guard(mtx);

  // lock3.unlock();
  std::cout << "----------------------------------------------------"
            << std::endl;

  std::mutex mtx2, mtx3;
  std::unique_lock<std::mutex> lock4(mtx2, std::defer_lock);
  std::unique_lock<std::mutex> lock5(mtx3, std::defer_lock);
  lock4.lock();
  std::cout << "lock4 acquired mtx2." << std::endl;
  // 等待 5 秒钟 时间
  std::this_thread::sleep_for(std::chrono::seconds(5));

  lock4.unlock();

  lock5.lock();
  std::cout << "lock5 acquired mtx3." << std::endl;
  std::this_thread::sleep_for(std::chrono::seconds(5));
  lock5.unlock();
  std::cout << "-----------------------------------------------------"
            << std::endl;
  std::thread t1(func1);
  std::thread t2(func2);

  t1.join();
  t2.join();
  std::cout << "Threads completed." << std::endl;
  return 0;
}