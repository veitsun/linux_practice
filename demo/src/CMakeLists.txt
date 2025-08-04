# select all cpp  in this directory
file(GLOB HELLOLIB_DEMO_SRC_POOL ${PROJECT_SOURCE_DIR}/demo/src/*.cpp)

# # 输出调试信息
# message(STATUS "Project source dir: ${PROJECT_SOURCE_DIR}")
# message(STATUS "Source files found: ${HELLOLIB_SRC_POOL}")

# create a  library from the collected source files
add_library(DEMO_LIB SHARED ${HELLOLIB_DEMO_SRC_POOL})
target_include_directories(DEMO_LIB PUBLIC ${INCLUDE_DIR_DEMO})

