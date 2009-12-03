include_directories(${CMAKE_SOURCE_DIR}/src ${QT_INCLUDES})

set(jsmokeqtxml_LIB_SRCS plugin.cpp qtxml.cpp qtxmlhandlers.cpp)

add_library(jsmokeqtxml MODULE ${jsmokeqtxml_LIB_SRCS})
target_link_libraries(  jsmokeqtxml 
                            ${QT_QTCORE_LIBRARY} 
                            ${QT_QTXML_LIBRARY} 
                            ${QT_QTSCRIPT_LIBRARY} 
                            smokeqtcore 
                            smokeqtxml 
                            jsmokeruntime )

set_target_properties(jsmokeqtxml PROPERTIES PREFIX "")

install(TARGETS jsmokeqtxml DESTINATION ${QT_PLUGINS_DIR}/script )

