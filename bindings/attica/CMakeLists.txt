include_directories(${CMAKE_SOURCE_DIR}/src ${QT_INCLUDES})

set(jsmokeattica_LIB_SRCS plugin.cpp attica.cpp atticahandlers.cpp)

add_library(jsmokeattica MODULE ${jsmokeattica_LIB_SRCS})
target_link_libraries(  jsmokeattica 
                            ${QT_QTCORE_LIBRARY} 
                            ${QT_QTSCRIPT_LIBRARY} 
                            ${LIBATTICA_LIBRARIES}
                            attica
                            smokeqtcore 
                            smokeattica 
                            jsmokeruntime )

set_target_properties(jsmokeattica PROPERTIES PREFIX "")

install(TARGETS jsmokeattica DESTINATION ${QT_PLUGINS_DIR}/script )
