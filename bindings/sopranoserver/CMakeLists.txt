include_directories(${CMAKE_SOURCE_DIR}/src ${QT_INCLUDES})

set(jsmokesopranoserver_LIB_SRCS plugin.cpp sopranoserver.cpp sopranoserverhandlers.cpp)

add_library(jsmokesopranoserver MODULE ${jsmokesopranoserver_LIB_SRCS})
target_link_libraries(  jsmokesopranoserver 
                            ${QT_QTCORE_LIBRARY} 
                            ${QT_QTSCRIPT_LIBRARY} 
                            ${SOPRANO_LIBRARIES}
                            smokeqtcore 
                            smokesopranoserver 
                            jsmokeruntime )

set_target_properties(jsmokesopranoserver PROPERTIES PREFIX "")

install(TARGETS jsmokesopranoserver DESTINATION ${QT_PLUGINS_DIR}/script )
