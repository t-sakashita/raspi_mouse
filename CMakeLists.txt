cmake_minimum_required(VERSION 2.8)

project(champion NONE)

enable_language(CXX C)

# test environment
enable_testing()
include(CTest)


include_directories(${PROJECT_SOURCE_DIR}/header)
include_directories(${CMAKE_SOURCE_DIR}/external/bcm2835/src)

add_subdirectory(external)

add_subdirectory(main_program)

add_subdirectory(test)
