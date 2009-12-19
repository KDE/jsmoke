include_directories(${CMAKE_SOURCE_DIR}/src ${QT_INCLUDES} ${SMOKE_INCLUDE_DIR})

set(jsmokekparts_LIB_SRCS plugin.cpp kparts.cpp kpartshandlers.cpp)

add_library(jsmokekparts MODULE ${jsmokekparts_LIB_SRCS})
target_link_libraries(  jsmokekparts 
                            ${QT_QTCORE_LIBRARY} 
                            ${QT_QTGUI_LIBRARY} 
                            ${QT_QTXML_LIBRARY} 
                            ${QT_QTSCRIPT_LIBRARY} 
                            ${SMOKE_QTCORE_LIBRARY} 
                            smokekparts 
                            jsmokeruntime )

set_target_properties(jsmokekparts PROPERTIES PREFIX "")

install(TARGETS jsmokekparts DESTINATION ${QT_PLUGINS_DIR}/script )
