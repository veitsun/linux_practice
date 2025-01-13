cmake_minimum_required(VERSION 3.16)
project(linux_practice CXX)

# Build type
set(CMAKE_BUILD_TYPE "Debug" )
Set(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g -ggdb -DDEBUG")
Set(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")
set(CMAKE_EXPORT_COMPILE_COMMANDS on)
include_directories(${INCLUDE_DIR})
add_subdirectory(src)
add_subdirectory(echo_server)

add_executable(main main.cpp)
target_include_directories(main PUBLIC ${INCLUDE_DIR})
# gcc -Wall test6.cpp -lpthread -lm -o test6
target_link_libraries(main PRIVATE SUNWEI pthread)