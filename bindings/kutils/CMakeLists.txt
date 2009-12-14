include_directories(${CMAKE_SOURCE_DIR}/src ${QT_INCLUDES} ${SMOKE_INCLUDE_DIR})

set(jsmokekutils_LIB_SRCS plugin.cpp kutils.cpp kutilshandlers.cpp)

add_library(jsmokekutils MODULE ${jsmokekutils_LIB_SRCS})
target_link_libraries(  jsmokekutils 
                            ${QT_QTCORE_LIBRARY} 
                            ${QT_QTGUI_LIBRARY} 
                            ${QT_QTSCRIPT_LIBRARY} 
                            ${SMOKE_QTCORE_LIBRARY} 
                            smokekutils 
                            jsmokeruntime )

set_target_properties(jsmokekutils PROPERTIES PREFIX "")

install(TARGETS jsmokekutils DESTINATION ${QT_PLUGINS_DIR}/script )
