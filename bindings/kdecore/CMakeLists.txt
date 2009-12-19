include_directories(${CMAKE_SOURCE_DIR}/src ${QT_INCLUDES} ${SMOKE_INCLUDE_DIR})

set(jsmokekdecore_LIB_SRCS plugin.cpp kdecore.cpp kdecorehandlers.cpp)

add_library(jsmokekdecore MODULE ${jsmokekdecore_LIB_SRCS})
target_link_libraries(  jsmokekdecore 
                            ${QT_QTCORE_LIBRARY} 
                            ${QT_QTNETWORK_LIBRARY} 
                            ${QT_QTGUI_LIBRARY} 
                            ${QT_QTSCRIPT_LIBRARY} 
                            ${KDE4_KDECORE_LIBS} 
                            ${SMOKE_QTCORE_LIBRARY} 
                            smokekdecore 
                            jsmokeruntime )

set_target_properties(jsmokekdecore PROPERTIES PREFIX "")

install(TARGETS jsmokekdecore DESTINATION ${QT_PLUGINS_DIR}/script )
