cmake_minimum_required(VERSION 3.0)

project(server)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_COMPILER /opt/rh/devtoolset-8/root/usr/bin/g++)
#set(CMAKE_CXX_EXTENSIONS OFF)

include_directories("/usr/include/mysql")
include_directories("/usr/include/jsoncpp")

include_directories(${CMAKE_SOURCE_DIR}/responsitories/MD2Html)

#add_subdirectory(${CMAKE_SOURCE_DIR}/responsitories/MD2Html)

file(GLOB_RECURSE md2html_files ${CMAKE_SOURCE_DIR}/responsitories/MD2Html/*)

set(header /usr/include/semaphore.h lock/locker.h /usr/include/mysql/mysql.h 
  CGImysql/sql_connection_pool.h
  http/http_conn.h log/block_queue.h log/log.h threadpool/threadpool.h
  timer/lst_timer.h config.h webserver.h )

set(src CGImysql/sql_connection_pool.cpp http/http_conn.cpp log/log.cpp timer/lst_timer.cpp
config.cpp main.cpp webserver.cpp)

link_directories(/www/server/mysql/lib/)

add_executable(server ${src} ${header} ${md2html_files})

target_link_libraries(server pthread mysqlclient jsoncpp curl)
