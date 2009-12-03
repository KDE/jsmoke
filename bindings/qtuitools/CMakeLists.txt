include_directories(${CMAKE_SOURCE_DIR}/src ${QT_INCLUDES})

set(jsmokeqtuitools_LIB_SRCS plugin.cpp qtuitools.cpp qtuitoolshandlers.cpp)

add_library(jsmokeqtuitools MODULE ${jsmokeqtuitools_LIB_SRCS})
target_link_libraries(  jsmokeqtuitools 
                            ${QT_QTCORE_LIBRARY} 
                            ${QT_QTGUI_LIBRARY} 
                            ${QT_QTUITOOLS_LIBRARY} 
                            ${QT_QTSCRIPT_LIBRARY} 
                            smokeqtcore 
                            smokeqtgui 
                            smokeqtuitools 
                            jsmokeruntime )

set_target_properties(jsmokeqtuitools PROPERTIES PREFIX "")

install(TARGETS jsmokeqtuitools DESTINATION ${QT_PLUGINS_DIR}/script )

