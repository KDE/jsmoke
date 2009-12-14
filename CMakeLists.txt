project(qtscript-smoke)
cmake_minimum_required(VERSION 2.6)

# where to look first for cmake modules, before ${CMAKE_ROOT}/Modules/ is checked
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/modules )

find_package(Qt4 REQUIRED)
find_package(SmokeQt REQUIRED)

add_subdirectory(bindings)
add_subdirectory(src)
add_subdirectory(tools)
