include_directories(${CMAKE_SOURCE_DIR}/src ${QT_INCLUDES} ${SMOKE_INCLUDE_DIR})

set(jsmokekdeui_LIB_SRCS plugin.cpp kdeui.cpp kdeuihandlers.cpp)

add_library(jsmokekdeui MODULE ${jsmokekdeui_LIB_SRCS})
target_link_libraries(  jsmokekdeui 
                            ${QT_QTCORE_LIBRARY} 
                            ${QT_QTGUI_LIBRARY} 
                            ${QT_QTSCRIPT_LIBRARY} 
                            ${SMOKE_QTCORE_LIBRARY} 
                            smokekdeui 
                            jsmokeruntime )

set_target_properties(jsmokekdeui PROPERTIES PREFIX "")

install(TARGETS jsmokekdeui DESTINATION ${QT_PLUGINS_DIR}/script )
