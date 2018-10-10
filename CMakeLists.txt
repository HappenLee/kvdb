cmake_minimum_required(VERSION 3.8.2)
project(ldb)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)
include_directories(include/common)
include_directories(include/skiplist)
include_directories(include/kvdb)


set(SOURCE_FILES  src/kvdb/kvdb.cpp
                  include/skiplist/node.h
                  include/common/common.h
                  include/skiplist/skiplist.h
                  include/kvdb/kvdb.h
                  src/skiplist/skiplist.cpp
                  src/kvdb/kvdb.cpp
                  src/skiplist/node.cpp)

set(LINK_LIB pthread glog gtest)

add_library(kvdb STATIC ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(kvdb ${LINK_LIB})

add_executable(main main.cpp ${SOURCE_FILES})
TARGET_LINK_LIBRARIES(main ${LINK_LIB})


FUNCTION(ADD_BE_TEST TEST_NAME)
    set(BUILD_OUTPUT_ROOT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/")
    # This gets the directory where the test is from (e.g. 'exprs' or 'runtime')
    get_filename_component(DIR_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)
    get_filename_component(TEST_DIR_NAME ${TEST_NAME} PATH)
    get_filename_component(TEST_FILE_NAME ${TEST_NAME} NAME)
    foreach(F ${SOURCE_FILES})
        set(SRC_LIST ${SRC_LIST} ../../${F})
    endforeach()
    ADD_EXECUTABLE(${TEST_FILE_NAME} ${TEST_NAME}.cpp ${SRC_LIST})
    TARGET_LINK_LIBRARIES(${TEST_FILE_NAME} ${LINK_LIB})
    SET_TARGET_PROPERTIES(${TEST_FILE_NAME} PROPERTIES COMPILE_FLAGS "-fno-access-control")
    ADD_TEST(${TEST_FILE_NAME} "${BUILD_OUTPUT_ROOT_DIRECTORY}/${TEST_NAME}")
ENDFUNCTION()

add_subdirectory(unittest/kvdb)
add_subdirectory(unittest/skiplist)


