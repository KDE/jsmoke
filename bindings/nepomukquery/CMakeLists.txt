include_directories(${CMAKE_SOURCE_DIR}/src ${QT_INCLUDES})

set(jsmokenepomukquery_LIB_SRCS plugin.cpp nepomukquery.cpp nepomukqueryhandlers.cpp)

add_library(jsmokenepomukquery MODULE ${jsmokenepomukquery_LIB_SRCS})
target_link_libraries(  jsmokenepomukquery 
                            ${QT_QTCORE_LIBRARY} 
                            ${QT_QTSCRIPT_LIBRARY} 
                            ${SOPRANO_LIBRARIES}
                            ${NEPOMUK_LIBRARIES}
                            nepomukquery
                            smokeqtcore 
                            smokenepomukquery 
                            jsmokeruntime )

set_target_properties(jsmokenepomukquery PROPERTIES PREFIX "")

install(TARGETS jsmokenepomukquery DESTINATION ${QT_PLUGINS_DIR}/script )
