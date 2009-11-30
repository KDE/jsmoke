include_directories(${CMAKE_SOURCE_DIR}/src ${QT_INCLUDES})

set(jsmokeqtgui_LIB_SRCS plugin.cpp qtgui.cpp qtguihandlers.cpp)

add_library(jsmokeqtgui MODULE ${jsmokeqtgui_LIB_SRCS})
target_link_libraries(  jsmokeqtgui 
                            ${QT_QTCORE_LIBRARY} 
                            ${QT_QTGUI_LIBRARY} 
                            ${QT_QTSCRIPT_LIBRARY} 
                            smokeqtcore 
                            smokeqtgui 
                            jsmokeruntime )

set_target_properties(jsmokeqtgui PROPERTIES PREFIX "")

install(TARGETS jsmokeqtgui DESTINATION ${QT_PLUGINS_DIR}/script )

