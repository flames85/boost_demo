cmake_minimum_required(VERSION 3.5)

project(boost_asio)

message(STATUS "start running cmake...")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(Boost_INCLUDE_DIR /usr/local/include) # 设置Boost头文件目录
set(Boost_LIBRARY_DIR /usr/local/lib)     # 设置Boost库文件目录
find_package(Boost 1.55.0 COMPONENTS system filesystem thread REQUIRED) # 找库
if(Boost_FOUND)
    message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
    message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
    message(STATUS "Boost_VERSION: ${Boost_VERSION}")
    include_directories(${Boost_INCLUDE_DIRS}) # 导入头文件目录
endif()

set(SOURCE_FILES main.cpp) # 设置原文件是哪些
add_executable(boost_asio ${SOURCE_FILES}) # 设置编译目标

if(Boost_FOUND)
    target_link_libraries(boost_asio ${Boost_LIBRARIES}) # 设置链接哪些库

    #set(Boost_USE_STATIC_LIBS        ON) # only find static libs
    #set(Boost_USE_MULTITHREADED      ON)
    #set(Boost_USE_STATIC_RUNTIME    OFF)

endif()




