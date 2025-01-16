cmake_minimum_required(VERSION 3.16)
project(linux_practice CXX C)
set(CMAKE_CXX_STANDARD 11)

# Build type
set(CMAKE_BUILD_TYPE "Debug" )
Set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb -DDEBUG")
Set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

# set(INCLUDE_DIR "./usr/include") 应该放绝对路径
set(INCLUDE_DIR ${CMAKE_SOURCE_DIR}/myusr/include)
list(APPEND INCLUDE_DIR ${CMAKE_SOURCE_DIR}/include)
set(LIB_DIR ${CMAKE_SOURCE_DIR}/myusr/lib)

set(CMAKE_EXPORT_COMPILE_COMMANDS on)

add_subdirectory(src)
add_subdirectory(echo_cliserver)
add_subdirectory(client)
add_subdirectory(server)

add_executable(main main.cpp)
target_include_directories(main PUBLIC ${INCLUDE_DIR})
target_link_libraries(main PRIVATE SUNWEI pthread)