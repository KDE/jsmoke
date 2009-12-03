include_directories(${CMAKE_SOURCE_DIR}/src ${QT_INCLUDES})

set(jsmokeqtsql_LIB_SRCS plugin.cpp qtsql.cpp qtsqlhandlers.cpp)

add_library(jsmokeqtsql MODULE ${jsmokeqtsql_LIB_SRCS})
target_link_libraries(  jsmokeqtsql 
                            ${QT_QTCORE_LIBRARY} 
                            ${QT_QTSQL_LIBRARY} 
                            ${QT_QTSCRIPT_LIBRARY} 
                            smokeqtcore 
                            smokeqtsql 
                            jsmokeruntime )

set_target_properties(jsmokeqtsql PROPERTIES PREFIX "")

install(TARGETS jsmokeqtsql DESTINATION ${QT_PLUGINS_DIR}/script )

