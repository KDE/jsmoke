include_directories(${CMAKE_SOURCE_DIR}/src ${QT_INCLUDES} ${SMOKE_INCLUDE_DIR})

set(jsmokektexteditor_LIB_SRCS plugin.cpp ktexteditor.cpp ktexteditorhandlers.cpp)

add_library(jsmokektexteditor MODULE ${jsmokektexteditor_LIB_SRCS})
target_link_libraries(  jsmokektexteditor 
                            ${QT_QTCORE_LIBRARY} 
                            ${QT_QTGUI_LIBRARY} 
                            ${QT_QTSCRIPT_LIBRARY} 
                            ${SMOKE_QTCORE_LIBRARY} 
                            ${KDE4_KTEXTEDITOR_LIBS} 
                            smokektexteditor 
                            jsmokeruntime )

set_target_properties(jsmokektexteditor PROPERTIES PREFIX "")

install(TARGETS jsmokektexteditor DESTINATION ${QT_PLUGINS_DIR}/script )
