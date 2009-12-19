include_directories(${CMAKE_SOURCE_DIR}/src ${QT_INCLUDES} ${SMOKE_INCLUDE_DIR})

set(jsmokekio_LIB_SRCS plugin.cpp kio.cpp kiohandlers.cpp)

add_library(jsmokekio MODULE ${jsmokekio_LIB_SRCS})
target_link_libraries(  jsmokekio 
                            ${QT_QTCORE_LIBRARY} 
                            ${QT_QTNETWORK_LIBRARY} 
                            ${QT_QTGUI_LIBRARY} 
                            ${QT_QTSCRIPT_LIBRARY} 
                            ${SMOKE_QTCORE_LIBRARY} 
                            ${KDE4_KDECORE_LIBS} 
                            ${KDE4_KIO_LIBS} 
                            smokekio 
                            jsmokeruntime )

set_target_properties(jsmokekio PROPERTIES PREFIX "")

install(TARGETS jsmokekio DESTINATION ${QT_PLUGINS_DIR}/script )
