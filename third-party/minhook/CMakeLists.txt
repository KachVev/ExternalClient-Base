add_library(minhook STATIC IMPORTED GLOBAL)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(MINHOOK_LIB "${CMAKE_CURRENT_SOURCE_DIR}/lib/x64/MinHook.lib")
else()
    set(MINHOOK_LIB "${CMAKE_CURRENT_SOURCE_DIR}/lib/x86/MinHook.lib")
endif()

set_target_properties(minhook PROPERTIES
        IMPORTED_LOCATION "${MINHOOK_LIB}"
        INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_SOURCE_DIR}/include"
)
