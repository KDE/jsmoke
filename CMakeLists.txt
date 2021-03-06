project(qtscript-smoke)
cmake_minimum_required(VERSION 2.6)

# where to look first for cmake modules, before ${CMAKE_ROOT}/Modules/ is checked
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake/modules )

find_package(Qt4 REQUIRED)
find_package(Smoke COMPONENTS QtCore QtGui QtXml QtOpenGl QtSql QtNetwork QtDbus QtSvg Phonon QSci QtDeclarative QtScript QtWebkit QtUiTools QtTest Qwt)

find_package(KDE4 REQUIRED)
include(KDE4Defaults)
include(MacroLibrary)

macro_optional_find_package(OpenGL)
macro_optional_find_package(Soprano)
macro_optional_find_package(Nepomuk)
macro_optional_find_package(Phonon)

add_definitions (${QT_DEFINITIONS} ${KDE4_DEFINITIONS} -DQT3_SUPPORT)
include_directories (${CMAKE_SOURCE_DIR} ${CMAKE_BINARY_DIR} ${KDE4_INCLUDES} ${SMOKE_INCLUDE_DIR})

add_subdirectory(bindings)
add_subdirectory(src)
add_subdirectory(tools)
