file(GLOB IMGUI_SOURCES
        ${CMAKE_CURRENT_SOURCE_DIR}/backends/imgui_impl_dx11.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/backends/imgui_impl_win32.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/imgui.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/imgui_demo.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/imgui_tables.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/imgui_widgets.cpp
)

add_library(imgui STATIC ${IMGUI_SOURCES})

target_include_directories(imgui PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}/backends
        ${CMAKE_CURRENT_SOURCE_DIR}/src
)
