include_directories(${CMAKE_SOURCE_DIR}/src ${QT_INCLUDES})

set(jsmokeqtmultimedia_LIB_SRCS plugin.cpp qtmultimedia.cpp qtmultimediahandlers.cpp)

add_library(jsmokeqtmultimedia MODULE ${jsmokeqtmultimedia_LIB_SRCS})
target_link_libraries(  jsmokeqtmultimedia 
                            ${QT_QTCORE_LIBRARY} 
                            ${QT_QTGUI_LIBRARY} 
                            ${QT_QTSCRIPT_LIBRARY} 
                            smokeqtcore 
                            smokeqtmultimedia 
                            jsmokeruntime )

set_target_properties(jsmokeqtmultimedia PROPERTIES PREFIX "")

install(TARGETS jsmokeqtmultimedia DESTINATION ${QT_PLUGINS_DIR}/script )
