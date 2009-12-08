include_directories(${CMAKE_SOURCE_DIR}/src ${QT_INCLUDES})

set(jsmokekfile_LIB_SRCS plugin.cpp kfile.cpp kfilehandlers.cpp)

add_library(jsmokekfile MODULE ${jsmokekfile_LIB_SRCS})
target_link_libraries(  jsmokekfile 
                            ${QT_QTCORE_LIBRARY} 
                            ${QT_QTGUI_LIBRARY} 
                            ${QT_QTSCRIPT_LIBRARY} 
                            smokeqtcore 
                            smokekfile 
                            jsmokeruntime )

set_target_properties(jsmokekfile PROPERTIES PREFIX "")

install(TARGETS jsmokekfile DESTINATION ${QT_PLUGINS_DIR}/script )
