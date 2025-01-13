# select all cpp  in this directory
file(GLOB HEllOLIB_SRC_POOL ${PROJECT_SOURCE_DIR}/src/*.cpp)

# 古老的 找 include 头文件的方法
# include_directories(${CMAKE_SOURCE_DIR}/include)
# 这个命令的作用是全局性的，也就是说，所有在此命令之后添加的目标（比如可执行文件，库等）都会使用这个指定的头文件路径。
# 这种做法可能会导致问题，特别是如果你只想让某个特定的目标使用这个路径的时候。因此，现代 cmake 推荐使用 target_include_directories 命令来指定头文件路径。
# find_path(INCLUDE_DIR CTest.h HINTS ${CMAKE_SOURCE_DIR}/include)
# if(NOT INCLUDE_DIR) 
#   message(FATAL_ERROR "CTest.h not found !")
# endif()
# include_directories(INCLUDE_DIR)


# # 输出调试信息
# message(STATUS "Project source dir: ${PROJECT_SOURCE_DIR}")
# message(STATUS "Source files found: ${HELLOLIB_SRC_POOL}")

# create a  library from the collected source files
add_library(SUNWEI SHARED ${HEllOLIB_SRC_POOL})
target_include_directories(SUNWEI PUBLIC ${INCLUDE_DIR})

