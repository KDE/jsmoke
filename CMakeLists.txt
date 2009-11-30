project(qtscript-smoke)
cmake_minimum_required(VERSION 2.6)
find_package(Qt4 REQUIRED)

add_subdirectory(bindings)
add_subdirectory(src)
add_subdirectory(tools)
