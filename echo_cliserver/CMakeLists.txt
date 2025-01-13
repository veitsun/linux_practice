# include_directories(${INCLUDE_DIR})


# 定义宏
macro(add_multiple_executables)
    foreach(executable_name ${ARGN})
        add_executable(${executable_name} ${executable_name}.cpp)
        target_include_directories(${executable_name} PUBLIC ${INCLUDE_DIR})
        target_link_libraries(${executable_name} PRIVATE SUNWEI)
    endforeach()
endmacro()

# 调用宏
add_multiple_executables(
  helloword
  tcpserver01
)