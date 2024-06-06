cmake_minimum_required(VERSION 3.10)

add_library(analysis analysis.cpp save_js.cpp)
target_include_directories(analysis PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})