include_directories(${CMAKE_SOURCE_DIR}/src ${QT_INCLUDES} ${SMOKE_INCLUDE_DIR})

set(jsmokeqtwebkit_LIB_SRCS plugin.cpp qtwebkit.cpp qtwebkithandlers.cpp)

add_library(jsmokeqtwebkit MODULE ${jsmokeqtwebkit_LIB_SRCS})
target_link_libraries(  jsmokeqtwebkit 
                            ${QT_QTCORE_LIBRARY} 
                            ${QT_QTNETWORK_LIBRARY} 
                            ${QT_QTGUI_LIBRARY} 
                            ${QT_QTWEBKIT_LIBRARY} 
                            ${QT_QTSCRIPT_LIBRARY} 
                            ${SMOKE_QTCORE_LIBRARY} 
                            smokeqtnetwork 
                            smokeqtgui 
                            smokeqtwebkit 
                            jsmokeruntime )

set_target_properties(jsmokeqtwebkit PROPERTIES PREFIX "")

install(TARGETS jsmokeqtwebkit DESTINATION ${QT_PLUGINS_DIR}/script )

