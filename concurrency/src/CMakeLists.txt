# select all cpp  in this directory
file(GLOB HEllOLIB_CONCURRENCY_SRC_POOL ${PROJECT_SOURCE_DIR}/concurrency/src/*.cpp)

# # 输出调试信息
# message(STATUS "Project source dir: ${PROJECT_SOURCE_DIR}")
# message(STATUS "Source files found: ${HELLOLIB_SRC_POOL}")

# create a  library from the collected source files
add_library(CONCURRENCY_LIB SHARED ${HEllOLIB_CONCURRENCY_SRC_POOL})
target_include_directories(CONCURRENCY_LIB PUBLIC ${INCLUDE_DIR_CON})

