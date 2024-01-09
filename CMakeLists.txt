# CMake 最低版本号要求
cmake_minimum_required (VERSION 3.15)

# 项目信息
project (Demo1)
set(CMAKE_CXX_STANDARD 17)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -m64")
# set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -m64")
SET(CMAKE_BUILD_TYPE "Debug") 

#添加 gtest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

#添加头文件
include_directories(${PROJECT_SOURCE_DIR}/include)

#设置cpp源文件
file(GLOB SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc)

#生成库文件
set(LIBRARY_OUTPUT_PATH ../lib)
add_library(archive SHARED ${SRC})


# 指定生成目标, Demo是要生成的可执行文件名，setup.cc是要编译的文件
add_executable(Demo setup.cpp)
target_link_libraries(Demo archive)
target_link_libraries(Demo stdc++fs)


add_executable(basicTest test/basicTest.cpp)
target_link_libraries(basicTest archive)
target_link_libraries(basicTest stdc++fs)
# 链接 Google Test 库
target_link_libraries(basicTest ${GTEST_LIBRARIES} pthread)


