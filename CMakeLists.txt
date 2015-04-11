cmake_minimum_required(VERSION 2.6)

project(coroutine)

set(CMAKE_CXX_FLAGS
-g
-Wconversion
-Wall
-Werror
-Wextra
-Wshadow
-Wno-unused-parameter
-Wold-style-cast
-Woverloaded-virtual
-Wpointer-arith
-Wwrite-strings
-Wreorder
-march=native
-D_FILE_OFFSET_BITS=64
-DCHECK_PTHREAD_RETURN_VALUE
-rdynamic
)

if(CMAKE_BUILD_BITS EQUAL 32)
    list(APPEND CMAKE_CXX_FLAGS "-m32")
endif()

string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

if(${CMAKE_BUILD_TYPE} STREQUAL "debug")
    set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib/debug)
elseif(${CMAKE_BUILD_TYPE} STREQUAL "release")
    set(LIBRARY_OUTPUT_PATH ${CMAKE_SOURCE_DIR}/lib/release)
else()
    message(FATAL_ERROR "Wrong build option! usage: -DCMAKE_BUILD_TYPE=Debug/Release")
endif()

find_package(Boost REQUIRED)
if(Boost_FOUND)
    message(STATUS "found Boost")
    include_directories(${Boost_INCLUDE_DIRS})
endif()

include_directories(src)

add_subdirectory(src)
add_subdirectory(test)
