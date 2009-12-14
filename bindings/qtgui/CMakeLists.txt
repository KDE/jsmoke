include_directories(${CMAKE_SOURCE_DIR}/src ${QT_INCLUDES} ${SMOKE_INCLUDE_DIR})

set(jsmokeqtgui_LIB_SRCS plugin.cpp qtgui.cpp qtguihandlers.cpp)

add_library(jsmokeqtgui MODULE ${jsmokeqtgui_LIB_SRCS})
target_link_libraries(  jsmokeqtgui 
                            ${QT_QTCORE_LIBRARY} 
                            ${QT_QTGUI_LIBRARY} 
                            ${QT_QTSCRIPT_LIBRARY} 
                            ${SMOKE_QTCORE_LIBRARY} 
                            smokeqtgui 
                            jsmokeruntime )

set_target_properties(jsmokeqtgui PROPERTIES PREFIX "")

install(TARGETS jsmokeqtgui DESTINATION ${QT_PLUGINS_DIR}/script )

