cmake_minimum_required(VERSION 3.10)

set(CMAKE_CXX_STANDARD 17)

project(native_mobile)

add_subdirectory(lib)
add_subdirectory(sample)

#include_directories("/usr/local/opt/icu4c/include")