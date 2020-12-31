cmake_minimum_required(VERSION 2.8)

include_directories(${CMAKE_SOURCE_DIR}/src)

project(bcm2835 NONE)

enable_language(C)

add_subdirectory(src)
add_subdirectory(examples)
