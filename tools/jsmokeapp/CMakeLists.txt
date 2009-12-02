include_directories(${QT_INCLUDES})

set(jsmokeapp_SRCS src/jsmokeapp.cpp src/common.cpp)
set(jsmokecoreapp_SRCS src/jsmokecoreapp.cpp src/common.cpp)

add_executable(jsmokeapp ${jsmokeapp_SRCS})
add_executable(jsmokecoreapp ${jsmokecoreapp_SRCS})

target_link_libraries(  jsmokeapp 
                            ${QT_QTCORE_LIBRARY} 
                            ${QT_QTGUI_LIBRARY} 
                            ${QT_QTSCRIPT_LIBRARY} )

target_link_libraries(  jsmokecoreapp 
                            ${QT_QTCORE_LIBRARY} 
                            ${QT_QTSCRIPT_LIBRARY} )

install(    TARGETS jsmokeapp 
            DESTINATION ${CMAKE_INSTALL_PREFIX}/bin )

install(    TARGETS jsmokecoreapp 
            DESTINATION ${CMAKE_INSTALL_PREFIX}/bin )


