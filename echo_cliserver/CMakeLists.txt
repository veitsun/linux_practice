# include_directories(${INCLUDE_DIR})

# 定义工程目录
SET(ROOT_DIR ${CMAKE_SOURCE_DIR})
LINK_DIRECTORIES(${ROOT_DIR}/myusr/lib)

# /home/sunwei/linux_practice/myusr/lib/libunp.a
# g++ tcpserver01.cpp -o server -I/home/sunwei/linux_practice/myusr/include     -L/home/sunwei/linux_practice/myusr/lib     -Wl,--start-group -lunp -lpthread -Wl,--end-group
# 定义宏
macro(add_multiple_executables)
    foreach(executable_name ${ARGN})
        add_executable(${executable_name} ${executable_name}.cpp)
        target_include_directories(${executable_name} PUBLIC ${INCLUDE_DIR})
        target_link_libraries(${executable_name} PRIVATE SUNWEI
                                                -Wl,--start-group
                                                unp
                                                pthread
                                                -Wl,--end-group
                                                )
    endforeach()
endmacro()

# 调用宏
add_multiple_executables(
  helloword
  tcpserver01
  tcpcli01
)