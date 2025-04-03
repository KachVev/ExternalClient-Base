cmake_minimum_required(VERSION 3.31)
project(CS2_External)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(third-party/memory)

add_executable(CS2_External src/main.cpp
        src/main.cpp
        src/math/vector2.hpp
        src/math/vector3.hpp
)

target_link_libraries(CS2_External memory)