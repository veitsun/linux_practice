cmake_minimum_required(VERSION 3.16)
project(linux_practice CXX)

# Build type
set(CMAKE_BUILD_TYPE "Debug" )
Set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb -DDEBUG")
Set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

include_directories(include)
add_subdirectory(src)

add_executable(main main.cpp)

# gcc -Wall test6.cpp -lpthread -lm -o test6
target_link_libraries(main PRIVATE SUNWEI pthread)