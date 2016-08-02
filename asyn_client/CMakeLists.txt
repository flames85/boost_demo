cmake_minimum_required(VERSION 3.5)

# 设置工程名称
project(asyn_client)

message(STATUS "start running cmake...")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_PREFIX_PATH ./find_package_config)

# 关于boost
set(Boost_INCLUDE_DIR /usr/local/include) # 设置Boost头文件目录
set(Boost_LIBRARY_DIR /usr/local/lib)     # 设置Boost库文件目录
find_package(Boost 1.55.0 COMPONENTS system filesystem thread program_options REQUIRED) # 找库
if(Boost_FOUND)
    message(STATUS "Boost_VERSION: ${Boost_VERSION}") # 版本
    message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}") # 头文件路径
    message(STATUS "Boost_LIBRARY_DIR: ${Boost_LIBRARY_DIR}") # 库文件路径
    message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}") # 库文件

    INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIRS})
    LINK_DIRECTORIES(${Boost_LIBRARY_DIR})
else()
    # Error; BOOST NOT FOUND!
endif()

# 关于openssl
set(OPENSSL_INCLUDE_DIRS /usr/local/include) # 设置openssl头文件目录
set(OPENSSL_LIBRARY_DIR /usr/lib) # 设置openssl头文件目录
find_package(OPENSSL REQUIRED openssl)
if( OPENSSL_FOUND )
    message(STATUS "OPENSSL_VERSION: ${OPENSSL_VERSION}")
    message(STATUS "OPENSSL_INCLUDE_DIRS: ${OPENSSL_INCLUDE_DIRS}")
    message(STATUS "OPENSSL_LIBRARY_DIR: ${OPENSSL_LIBRARY_DIR}")
    message(STATUS "OPENSSL_LIBRARIES: ${OPENSSL_LIBRARY_DIR}")

    INCLUDE_DIRECTORIES(${OPENSSL_INCLUDE_DIRS})
    LINK_DIRECTORIES(${OPENSSL_LIBRARY_DIR})
else()
    # Error; OPENSSL NOT FOUND!
endif()


# log4cxx
set(LOG4CXX_INCLUDE_DIRS /usr/local/Cellar/log4cxx/0.10.0/include) # 设置logcxx头文件目录
set(LOG4CXX_LIBRARY_DIR /usr/local/Cellar/log4cxx/0.10.0/lib) # 设置logcxx头文件目录
message(STATUS "CMAKE_MODULE_PATH: ${CMAKE_MODULE_PATH}" )

find_package(LOG4CXX REQUIRED log4cxx)
if( LOG4CXX_FOUND )
    message(STATUS "LOG4CXX_VERSION: ${LOG4CXX_VERSION}")
    message(STATUS "LOG4CXX_INCLUDE_DIRS: ${LOG4CXX_INCLUDE_DIRS}")
    message(STATUS "LOG4CXX_LIBRARY_DIR: ${LOG4CXX_LIBRARY_DIR}")
    message(STATUS "LOG4CXX_LIBRARIES: ${LOG4CXX_LIBRARIES}")

    INCLUDE_DIRECTORIES(${LOG4CXX_INCLUDE_DIRS})
    LINK_DIRECTORIES(${LOG4CXX_LIBRARY_DIR})
else()
    # Error; LOG4CXX NOT FOUND!
endif()



# 添加源文件
set(SOURCE_FILES
        main.cpp
        user_asyn_tcp_client.cpp
        user_asyn_tcp_client.h
        asyn_tcp_client.cpp
        asyn_tcp_client.h
        config_parser.cpp
        config_parser.h
        openssl_tool.cpp
        openssl_tool.h
        run_timer.h
        run_timer.hpp
        log_for_cxx.cpp
        log_for_cxx.h) # 设置原文件是哪些

# 设置编译目标
add_executable(asyn_client ${SOURCE_FILES})

# 设置链接哪些库
target_link_libraries(asyn_client ${Boost_LIBRARIES} ${OPENSSL_LIBRARIES} ${LOG4CXX_LIBRARIES} )



