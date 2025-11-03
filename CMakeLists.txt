cmake_minimum_required(VERSION 3.5)
project(Barnes-Hut-simulation
  LANGUAGES CXX
  VERSION 1.0
)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})


add_subdirectory(libs)
add_subdirectory(src)
target_include_directories(Barnes-Hut-simulation PRIVATE include)
