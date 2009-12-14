include_directories(${CMAKE_SOURCE_DIR}/src ${QT_INCLUDES} ${SMOKE_INCLUDE_DIR})

set(jsmokekhtml_LIB_SRCS plugin.cpp khtml.cpp khtmlhandlers.cpp)

add_library(jsmokekhtml MODULE ${jsmokekhtml_LIB_SRCS})
target_link_libraries(  jsmokekhtml 
                            ${QT_QTCORE_LIBRARY} 
                            ${QT_QTGUI_LIBRARY} 
                            ${QT_QTSCRIPT_LIBRARY} 
                            ${SMOKE_QTCORE_LIBRARY} 
                            smokekhtml 
                            jsmokeruntime )

set_target_properties(jsmokekhtml PROPERTIES PREFIX "")

install(TARGETS jsmokekhtml DESTINATION ${QT_PLUGINS_DIR}/script )
