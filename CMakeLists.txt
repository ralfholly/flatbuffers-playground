cmake_minimum_required(VERSION 3.18)

project(protobuf-playground)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

include(${CMAKE_BINARY_DIR}/conan_paths.cmake)

add_subdirectory(src)
