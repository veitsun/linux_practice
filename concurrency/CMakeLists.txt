set(INCLUDE_DIR_CON ${CMAKE_SOURCE_DIR}/myusr/include)
list(APPEND INCLUDE_DIR_CON ${CMAKE_SOURCE_DIR}/concurrency/src/include)
set(LIB_DIR ${CMAKE_SOURCE_DIR}/myusr/lib)

# 构建子目录
add_subdirectory(${CMAKE_SOURCE_DIR}/concurrency/src)

# /home/sunwei/linux_practice/myusr/lib/libunp.a
# g++ tcpserver01.cpp -o server -I/home/sunwei/linux_practice/myusr/include     -L/home/sunwei/linux_practice/myusr/lib     -Wl,--start-group -lunp -lpthread -Wl,--end-group
# 定义宏
macro(add_multiple_executables)
    foreach(executable_name ${ARGN})
        add_executable(${executable_name} ${executable_name}.cpp)
        target_include_directories(${executable_name} PUBLIC ${INCLUDE_DIR_CON})
        target_link_directories(${executable_name} PUBLIC ${LIB_DIR})
        target_link_libraries(${executable_name} PRIVATE CONCURRENCY_LIB
                                                -Wl,--start-group
                                                unp
                                                pthread
                                                -Wl,--end-group
                                                )
    endforeach()
endmacro()

# 调用宏
add_multiple_executables(
  test_concurrency_num
  test_multithread_singlethread
  test_thread_id
  test_thread_safe_stack
  test_hierarchical_mutex
  test_threadsafe_queue
  test_unique_lock
  test_thread_safe_queue_con
)