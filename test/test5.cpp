#include <chrono>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <thread>

std::condition_variable cv; // 同步变量
std::mutex cv_m;            // 互斥变量
int i = 0;

void waits() {
  std::unique_lock<std::mutex> lk(cv_m);
  std::cerr << "Waiting... \n";
  cv.wait(lk, [] {
    std::cerr << "wait go under wait\n";
    return i == 1;
  });
  std::cerr << "...finished waiting. i == 1\n";
}

void signals() {
  std::this_thread::sleep_for(std::chrono::seconds(1));
  {
    std::lock_guard<std::mutex> lk(cv_m);
    std::cerr << "Notifying...\n";
  }
  cv.notify_all();

  std::this_thread::sleep_for(std::chrono::seconds(1));

  {
    std::lock_guard<std::mutex> lk(cv_m);
    i = 1;
    std::cerr << "Notifying again...\n";
  }
  cv.notify_all();
}

int main() {
  std::thread t1(waits), t2(waits), t3(waits), t4(signals);
  t1.join();
  t2.join();
  t3.join();
  t4.join();
}