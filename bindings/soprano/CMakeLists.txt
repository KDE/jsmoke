include_directories(${CMAKE_SOURCE_DIR}/src ${QT_INCLUDES})

set(jsmokesoprano_LIB_SRCS plugin.cpp soprano.cpp sopranohandlers.cpp)

add_library(jsmokesoprano MODULE ${jsmokesoprano_LIB_SRCS})
target_link_libraries(  jsmokesoprano 
                            ${QT_QTCORE_LIBRARY} 
                            ${QT_QTSCRIPT_LIBRARY}
                            ${SOPRANO_LIBRARIES}
                            smokeqtcore 
                            smokesoprano 
                            jsmokeruntime )

set_target_properties(jsmokesoprano PROPERTIES PREFIX "")

install(TARGETS jsmokesoprano DESTINATION ${QT_PLUGINS_DIR}/script )
