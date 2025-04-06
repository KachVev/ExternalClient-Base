add_library(memory STATIC src/Memory.cpp)

target_include_directories(memory PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
