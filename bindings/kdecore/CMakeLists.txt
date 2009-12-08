include_directories(${CMAKE_SOURCE_DIR}/src ${QT_INCLUDES})

set(jsmokekdecore_LIB_SRCS plugin.cpp kdecore.cpp kdecorehandlers.cpp)

add_library(jsmokekdecore MODULE ${jsmokekdecore_LIB_SRCS})
target_link_libraries(  jsmokekdecore 
                            ${QT_QTCORE_LIBRARY} 
                            ${QT_QTGUI_LIBRARY} 
                            ${QT_QTSCRIPT_LIBRARY} 
                            smokeqtcore 
                            smokekdecore 
                            jsmokeruntime )

set_target_properties(jsmokekdecore PROPERTIES PREFIX "")

install(TARGETS jsmokekdecore DESTINATION ${QT_PLUGINS_DIR}/script )
