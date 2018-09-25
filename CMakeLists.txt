cmake_minimum_required(VERSION 3.8.2)
project(ldb)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)
include_directories(include/common)
include_directories(include/skiplist)
include_directories(include/kvdb)

add_executable(ldb main.cpp include/skiplist/node.h
                            include/common/common.h
                            include/skiplist/skiplist.h
                            include/kvdb/kvdb.h
                            src/skiplist/skiplist.cpp
                            src/kvdb/kvdb.cpp)