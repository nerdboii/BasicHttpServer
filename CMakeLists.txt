cmake_minimum_required(VERSION 3.10)

project(SimpleHttpServer
    DESCRIPTION "A simple web server that supports HTTP/1.1"
    LANGUAGES CXX)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

include_directories(include)

set(SRC_DIR src)

add_executable(SimpleHttpServer
    ${SRC_DIR}/*.cpp
)

target_link_libraries(SimpleHttpServer PRIVATE Threads::Threads)
