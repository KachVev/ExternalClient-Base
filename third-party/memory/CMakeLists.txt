add_library(memory STATIC
        memory.cpp
        include/Signature.hpp
)

target_include_directories(memory PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
