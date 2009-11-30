include_directories(${CMAKE_SOURCE_DIR}/src ${QT_INCLUDES})

set(jsmokeqtnetwork_LIB_SRCS plugin.cpp qtnetwork.cpp qtnetworkhandlers.cpp)

add_library(jsmokeqtnetwork MODULE ${jsmokeqtnetwork_LIB_SRCS})
target_link_libraries(  jsmokeqtnetwork 
                            ${QT_QTCORE_LIBRARY} 
                            ${QT_QTNETWORK_LIBRARY} 
                            ${QT_QTSCRIPT_LIBRARY} 
                            smokeqtcore 
                            smokeqtnetwork 
                            jsmokeruntime )

set_target_properties(jsmokeqtnetwork PROPERTIES PREFIX "")

install(TARGETS jsmokeqtnetwork DESTINATION ${QT_PLUGINS_DIR}/script )

