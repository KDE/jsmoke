include_directories(${CMAKE_SOURCE_DIR}/src ${QT_INCLUDES} ${SMOKE_INCLUDE_DIR})

set(jsmokeqtdbus_LIB_SRCS plugin.cpp qtdbus.cpp qtdbushandlers.cpp)

add_library(jsmokeqtdbus MODULE ${jsmokeqtdbus_LIB_SRCS})
target_link_libraries(  jsmokeqtdbus 
                            ${QT_QTCORE_LIBRARY} 
                            ${QT_QTDBUS_LIBRARY} 
                            ${QT_QTSCRIPT_LIBRARY} 
                            ${SMOKE_QTCORE_LIBRARY} 
                            smokeqtdbus 
                            jsmokeruntime )

set_target_properties(jsmokeqtdbus PROPERTIES PREFIX "")

install(TARGETS jsmokeqtdbus DESTINATION ${QT_PLUGINS_DIR}/script )

