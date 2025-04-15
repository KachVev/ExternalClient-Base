cmake_minimum_required(VERSION 3.31)
project(CS_Internal)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_GENERATOR_PLATFORM x64)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

add_subdirectory(third-party/memory)
add_subdirectory(third-party/minhook)
add_subdirectory(third-party/imgui)

add_library(CS_Internal SHARED src/main.cpp
        src/hooks/hook.hpp
        src/hooks/hook.cpp
        src/render/imgui_layer.hpp
        src/render/imgui_layer.cpp
        src/ui/menu.cpp
        src/ui/menu.hpp
        src/fonts/IconsFontAwesome6.h
        src/fonts/awesomeFont.h
)

target_link_libraries(CS_Internal
        memory
        minhook
        imgui
        d3d11
        dxgi
        dxguid
        user32
)

target_include_directories(CS_Internal PRIVATE
        third-party/memory/include
        third-party/minhook/include
        third-party/imgui/include
        third-party/imgui/include/backends
)