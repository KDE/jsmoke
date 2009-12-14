include_directories(${CMAKE_SOURCE_DIR}/src ${QT_INCLUDES} ${SMOKE_INCLUDE_DIR})

set(jsmokeqtsvg_LIB_SRCS plugin.cpp qtsvg.cpp qtsvghandlers.cpp)

add_library(jsmokeqtsvg MODULE ${jsmokeqtsvg_LIB_SRCS})
target_link_libraries(  jsmokeqtsvg 
                            ${QT_QTCORE_LIBRARY} 
                            ${QT_QTGUI_LIBRARY} 
                            ${QT_QTSVG_LIBRARY} 
                            ${QT_QTSCRIPT_LIBRARY} 
                            ${SMOKE_QTCORE_LIBRARY} 
                            smokeqtgui 
                            smokeqtsvg 
                            jsmokeruntime )

set_target_properties(jsmokeqtsvg PROPERTIES PREFIX "")

install(TARGETS jsmokeqtsvg DESTINATION ${QT_PLUGINS_DIR}/script )

