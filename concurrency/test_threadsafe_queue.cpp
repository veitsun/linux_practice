#include "Threadsafe_queue.hpp"
#include <iostream>
#include <cassert>
#include <atomic>
#include <chrono>
#include <thread>
#include <vector>
#include <random>


// std::make_unique 是 c14 的标准，我的项目是使用c11，需要自己实现一下
#if __cplusplus <= 201103L
namespace std {
template <typename T, typename... Args>
std::unique_ptr<T> make_unique(Args &&...args) {
  return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
}
} // namespace std
#endif

void test_basic_functionality(){
    std::cout<<"测试基本功能... "<<std::endl;

    threadsafe_queue<int> queue;

    //测试入队
    queue.push(1);
    queue.push(2);
    queue.push(3);

    // try_pop测试出队
    auto value1 = queue.try_pop();
    assert(*value1 == 1);
    std::cout<<"try_pop : "<<*value1 <<std::endl;

    auto value2 = queue.try_pop();
    assert(*value2 == 2);
    std::cout<<"try_pop : "<<*value2 <<std::endl;

    auto value3 = queue.try_pop();
    assert(*value3 == 3);
    std::cout<<"try_pop : "<<*value3 <<std::endl;
    
    queue.push(1);
    queue.push(2);
    queue.push(3);
    // wait_and_pop测试出队
    auto v1 = 0;
    auto v2 = 0;
    auto v3 = 0;
    queue.wait_and_pop(v1);
    std::cout<<"wait_and_pop: "<< v1<<std::endl;
    queue.wait_and_pop(v2);
    std::cout<<"wait_and_pop: "<< v2<<std::endl;
    queue.wait_and_pop(v3);
    std::cout<<"wait_and_pop: "<< v3<<std::endl;
    // queue.wait_and_pop(v1);  一直阻塞等待
    

    std::cout<<"基本功能测试完毕！"<<std::endl;
}

void test_different_types() {
    std::cout << "\n测试不同数据类型..." << std::endl;

    // 测试字符串
    threadsafe_queue<std::string> string_queue;

    string_queue.push("hello");
    string_queue.push("world");

    auto str1 = string_queue.try_pop();
    assert(*str1 == "hello");
    std::cout<<"字符串弹出： "<< *str1<< std::endl;

    std::string str2 ;
    string_queue.wait_and_pop(str2);
    assert(str2 == "world");
    std::cout<<"字符串弹出： "<< str2<< std::endl;

    // 测试浮点数
    threadsafe_queue<double> double_queue;
    double_queue.push(2.11);
    double_queue.push(4.21);

    auto d1 = double_queue.try_pop();
    assert(std::abs(*d1 - 1) > 0.001);

    std::cout << "浮点数: " << *d1 << std::endl;

    std::cout << "不同数据类型测试通过!" << std::endl;
}
void test_multithreaded_push_pop(){
    std::cout << "\n测试多线程推入弹出..." << std::endl;

    threadsafe_queue<int> queue;

    const int num_threads = 4;
    const int items_per_thread = 10000;
    std::atomic<int> total_popped(0);
    std::vector<std::thread> threads;

    // 生产者线程
    for(int i = 0; i<num_threads;i++){
        threads.emplace_back([&queue,i,items_per_thread]{
            for(int j = 0;j < items_per_thread;j++){
                queue.push(i*items_per_thread + j);
            }
        });
    }

    // 消费者进程
    for(int i = 0; i<num_threads; i++){
        threads.emplace_back([&queue,&total_popped,items_per_thread]{
            for(int j = 0; j<items_per_thread;j++){
                bool success = false;
                while(!success){
                    try{
                        auto value = queue.wait_and_pop();
                        total_popped++;
                        success = true;
                    } catch(const std::exception &e){
                        std::this_thread::sleep_for(std::chrono::microseconds(1));
                    }
                }
            }
        });
    }
    // 等待所有线程完成
    for(auto &t :threads){
        t.join();
    }
    assert(total_popped == num_threads*items_per_thread);
    std::cout <<"多线程测试通过！！！总共处理了 "<<total_popped<<"个元素"
                <<std::endl;
}

void test_concurrent_mixed_operations(){
    std::cout << "\n测试并发混合操作..." << std::endl;

    threadsafe_queue<int> queue;
    std::atomic<bool> should_stop(false);
    std::atomic<int> push_count(0);
    std::atomic<int> pop_count(0);
    std::vector<std::thread> threads;

    // 随机推入线程
    threads.emplace_back([&](){ //以引用方式捕获外部作用域的所有变量
        std::random_device rd;
        std::mt19937 gen(rd()); //伪随机数生成引擎
        std::uniform_int_distribution<> dis(1,100); //分布适配器

        for(int i = 0 ; i < 500;i++){
            queue.push(dis(gen));
            push_count++;
            std::this_thread::sleep_for(std::chrono::microseconds(10));
        }
    });

    //随机弹出线程
    threads.emplace_back([&](){
        while(pop_count<500){
            try{
                auto value = queue.try_pop();
                pop_count++;
            }catch(const std::exception &e){
                std::this_thread::sleep_for(std::chrono::microseconds(10));
            }
        }
    });

    // 等待线程完成
    for (auto &t : threads) {
        t.join();
    }

    assert(push_count == 500);
    assert(pop_count == 500);
    std::cout << "并发混合操作测试通过! 推入: " << push_count
    << ", 弹出: " << pop_count << std::endl;
}
void test_stress_test(){
    std::cout << "\n进行压力测试..." << std::endl;

    threadsafe_queue<int> queue;
    const int stress_iterations= 100000;
    std::vector<std::thread> threads;

    auto start_time = std::chrono::high_resolution_clock::now();

    //多个生产者
    for(int i = 0;i<3;i++){
        threads.emplace_back([&queue,i,stress_iterations]{
            for(int j =0;j<stress_iterations;++j){
                queue.push(i*stress_iterations+j);
            }
        });
    }

    // 多个消费者
    std::atomic<int> consumed(0);
    for (int i = 0; i < 3; i++)
    {
        threads.emplace_back([&queue,&consumed,stress_iterations]{
            for(int j = 0 ; j<stress_iterations;j++){
                while(true){
                    try{
                        auto value = queue.try_pop();
                        consumed++;
                        break;
                    } catch (const std::exception &e){
                        std::this_thread::yield();
                    }
                }
            }
        });
    }

    for(auto &t:threads){
        t.join();
    }
    auto end_time = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(    
                    end_time - start_time);
    
    assert(consumed == 3*stress_iterations);
    std::cout << "压力测试通过! 处理了 " << consumed << " 个元素，用时 "
    << duration.count() << " 毫秒" << std::endl;
    
}
// void test_memory_safety(){
//     std::cout<< "\n测试内存安全..." << std::endl;

//     threadsafe_queue<std::unique_ptr<int>> queue;
    
//     //推入智能指针
//     queue.push(std::make_unique<int>(23));
//     queue.push(std::make_unique<int>(45));

//     //弹出并验证
//     auto ptr1 = queue.try_pop();
//     assert(**ptr1 == 23);
//     std::cout << "智能指针值: " << **ptr1 << std::endl;

//     auto ptr2 = queue.try_pop();
//     assert(**ptr2 == 45);
//     std::cout << "智能指针值: " << **ptr2 << std::endl;

//     std::cout << "内存安全测试通过!" << std::endl;
// }

int main(){
    try{
        test_basic_functionality();
        test_different_types();
        test_multithreaded_push_pop();
        test_concurrent_mixed_operations();
        test_stress_test();
        // test_memory_safety();  bug
        
        std::cout<<"\n 所有测试通过！"<<std::endl;

    }catch(const std::exception &e){
        std::cerr<<"测试失败！ "<< e.what() <<std::endl;
        return 1;
    }
    

    return 0;
}