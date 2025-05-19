#include <cstdio>
#include <sstream>
#include <thread>
#include <unordered_map>

std::unordered_map<std::thread::id, std::string> thread_map;

void threadFunction() {
  std::thread::id this_id = std::this_thread::get_id();
  std::stringstream ss;
  ss << "Thread ID: " << this_id;
  std::string thread_id_str = ss.str();
  thread_map[this_id] = thread_id_str;
}

int main() {
  std::thread t1(threadFunction);
  std::thread t2(threadFunction);
  t1.join();
  t2.join();

  for (const auto &pair : thread_map) {
    printf("%s\n", pair.second.c_str());
  }
  return 0;
}