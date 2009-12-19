include_directories(${CMAKE_SOURCE_DIR}/src ${QT_INCLUDES})

set(jsmokenepomuk_LIB_SRCS plugin.cpp nepomuk.cpp nepomukhandlers.cpp)

add_library(jsmokenepomuk MODULE ${jsmokenepomuk_LIB_SRCS})
target_link_libraries(  jsmokenepomuk 
                            ${QT_QTCORE_LIBRARY} 
                            ${QT_QTSCRIPT_LIBRARY} 
                            ${SOPRANO_LIBRARIES}
                            ${NEPOMUK_LIBRARIES}
                            smokeqtcore 
                            smokenepomuk 
                            jsmokeruntime )

set_target_properties(jsmokenepomuk PROPERTIES PREFIX "")

install(TARGETS jsmokenepomuk DESTINATION ${QT_PLUGINS_DIR}/script )
