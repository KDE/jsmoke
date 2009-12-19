include_directories(${CMAKE_SOURCE_DIR}/src ${QT_INCLUDES})

set(jsmokesopranoclient_LIB_SRCS plugin.cpp sopranoclient.cpp sopranoclienthandlers.cpp)

add_library(jsmokesopranoclient MODULE ${jsmokesopranoclient_LIB_SRCS})
target_link_libraries(  jsmokesopranoclient 
                            ${QT_QTCORE_LIBRARY} 
                            ${QT_QTSCRIPT_LIBRARY} 
                            ${SOPRANO_LIBRARIES}
                            smokeqtcore 
                            smokesopranoclient 
                            jsmokeruntime )

set_target_properties(jsmokesopranoclient PROPERTIES PREFIX "")

install(TARGETS jsmokesopranoclient DESTINATION ${QT_PLUGINS_DIR}/script )
