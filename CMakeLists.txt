cmake_minimum_required(VERSION 3.0)

project(server)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories("/usr/include/mysql")

set(header /usr/include/semaphore.h lock/locker.h /usr/include/mysql/mysql.h 
  CGImysql/sql_connection_pool.h
  http/http_conn.h log/block_queue.h log/log.h threadpool/threadpool.h
  timer/lst_timer.h config.h webserver.h )

set(src CGImysql/sql_connection_pool.cpp http/http_conn.cpp log/log.cpp timer/lst_timer.cpp
config.cpp main.cpp webserver.cpp)

#link_directories(/usr/lib/mysql)

add_executable(server ${src} ${header})

target_link_libraries(server pthread mysqlclient jsoncpp curl)
