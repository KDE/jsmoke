include_directories(${QT_INCLUDES})

set(qsexec_SRCS src/main.cpp)

add_executable(qsexec ${qsexec_SRCS})

target_link_libraries(  qsexec 
                            ${QT_QTCORE_LIBRARY} 
                            ${QT_QTGUI_LIBRARY} 
                            ${QT_QTSCRIPT_LIBRARY} )

install(    TARGETS qsexec 
            DESTINATION ${CMAKE_INSTALL_PREFIX}/bin )


