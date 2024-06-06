cmake_minimum_required(VERSION 3.10)

add_library(openai openai.hpp)
target_include_directories(openai PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})