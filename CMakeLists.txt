cmake_minimum_required(VERSION 3.31)
project(CS2_External)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(third-party/memory)

add_executable(CS2_External src/main.cpp)

target_link_libraries(CS2_External memory)

target_include_directories(CS2_External PRIVATE third-party/memory/include)
