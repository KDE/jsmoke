project(qtscript-smoke)
cmake_minimum_required(VERSION 2.6)

# where to look first for cmake modules, before ${CMAKE_ROOT}/Modules/ is checked
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/modules )

find_package(Qt4 REQUIRED)
find_package(SmokeQt REQUIRED)

find_package(KDE4 REQUIRED)
include(KDE4Defaults)
include(MacroLibrary)

macro_optional_find_package(OpenGL)
macro_optional_find_package(Soprano)
macro_optional_find_package(Nepomuk)
macro_optional_find_package(Phonon)

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS} -DQT3_SUPPORT)
include_directories (${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} ${KDE4_INCLUDES})

add_subdirectory(bindings)
add_subdirectory(src)
add_subdirectory(tools)
