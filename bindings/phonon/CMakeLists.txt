include_directories(${CMAKE_SOURCE_DIR}/src ${QT_INCLUDES})

set(jsmokephonon_LIB_SRCS plugin.cpp phonon.cpp phononhandlers.cpp)

add_library(jsmokephonon MODULE ${jsmokephonon_LIB_SRCS})
target_link_libraries(  jsmokephonon 
                            ${QT_QTCORE_LIBRARY} 
                            ${QT_QTSCRIPT_LIBRARY} 
                            ${PHONON_LIBS}
                            smokeqtcore 
                            smokephonon 
                            jsmokeruntime )

set_target_properties(jsmokephonon PROPERTIES PREFIX "")

install(TARGETS jsmokephonon DESTINATION ${QT_PLUGINS_DIR}/script )
