cmake_minimum_required(VERSION 3.0.0)

project(zlibwasm)

find_package(ZLIB REQUIRED)

option(ENABLE_ES6 "Export as a ES6 module" ON)

add_link_options("SHELL:-s WASM=1")
add_link_options("SHELL:-s EXPORTED_RUNTIME_METHODS='[\"cwrap\"]'")
add_link_options("SHELL:-s EXPORTED_FUNCTIONS='[\"_malloc\", \"_free\"]'")
add_link_options("SHELL:--post-js \"${CMAKE_CURRENT_SOURCE_DIR}/zlib_api.js\"")
add_link_options("SHELL:--closure 1")
if (ENABLE_ES6)
    add_link_options("SHELL:-s EXPORT_ES6=1 -s MODULARIZE=1")
endif()

add_executable(zlib zlib.c)
include_directories(${ZLIB_INCLUDE_DIRS})
target_link_libraries(zlib ZLIB::ZLIB)
