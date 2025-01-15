# set(INCLUDE_DIR "./usr/include") 应该放绝对路径
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/myusr/include)
list(APPEND INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(LIB_DIR ${CMAKE_SOURCE_DIR}/myusr/lib)

# 定义工程目录
# SET(ROOT_DIR ${CMAKE_SOURCE_DIR})
# LINK_DIRECTORIES(${ROOT_DIR}/myusr/lib)

# /home/sunwei/linux_practice/myusr/lib/libunp.a
# g++ tcpserver01.cpp -o server -I/home/sunwei/linux_practice/myusr/include     -L/home/sunwei/linux_practice/myusr/lib     -Wl,--start-group -lunp -lpthread -Wl,--end-group
# 定义宏
macro(add_multiple_executables)
    foreach(executable_name ${ARGN})
        add_executable(${executable_name} ${executable_name}.cpp)
        target_include_directories(${executable_name} PUBLIC ${INCLUDE_DIR})
        target_link_directories(${executable_name} PUBLIC ${LIB_DIR})
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
  tcpserver01
  tcpcli01
)