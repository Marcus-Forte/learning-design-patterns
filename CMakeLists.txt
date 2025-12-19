cmake_minimum_required(VERSION 3.20)
project(learning-design-patterns LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
include(format)
include(findGRPC)
include(generate_proto)

add_subdirectory(src/patterns)
add_subdirectory(src/solid)
