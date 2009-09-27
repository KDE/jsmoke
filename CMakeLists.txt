project(qtscript-smoke)
cmake_minimum_required(VERSION 2.6)
find_package(Qt4 REQUIRED)

include_directories(${QT_INCLUDES} ${CMAKE_CURRENT_BINARY_DIR})

set(qtscript-smoke_SRCS qtscript-smoke.cpp 
    main.cpp 
    StaticClass.cpp
    ImplementationClass.cpp
    QtScriptSmokeBinding.cpp  
    SmokeQtScriptUtils.cpp
    methodcall.cpp
    methodreturnvalue.cpp
    virtualmethodcall.cpp
    virtualmethodreturnvalue.cpp
    handlers.cpp )
qt4_automoc(${qtscript-smoke_SRCS})
add_executable(qtscript-smoke ${qtscript-smoke_SRCS})
target_link_libraries(qtscript-smoke ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} ${QT_QTSCRIPT_LIBRARY} smokeqt )
