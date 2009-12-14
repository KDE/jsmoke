include_directories(${CMAKE_SOURCE_DIR}/src ${QT_INCLUDES} ${SMOKE_INCLUDE_DIR})

set(jsmokekfile_LIB_SRCS plugin.cpp kfile.cpp kfilehandlers.cpp)

add_library(jsmokekfile MODULE ${jsmokekfile_LIB_SRCS})
target_link_libraries(  jsmokekfile 
                            ${QT_QTCORE_LIBRARY} 
                            ${QT_QTGUI_LIBRARY} 
                            ${QT_QTSCRIPT_LIBRARY} 
                            ${SMOKE_QTCORE_LIBRARY} 
                            smokekfile 
                            jsmokeruntime )

set_target_properties(jsmokekfile PROPERTIES PREFIX "")

install(TARGETS jsmokekfile DESTINATION ${QT_PLUGINS_DIR}/script )
