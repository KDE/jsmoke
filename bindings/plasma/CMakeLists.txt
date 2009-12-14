include_directories(${CMAKE_SOURCE_DIR}/src ${QT_INCLUDES} ${SMOKE_INCLUDE_DIR})

set(jsmokeplasma_LIB_SRCS plugin.cpp plasma.cpp plasmahandlers.cpp)

add_library(jsmokeplasma MODULE ${jsmokeplasma_LIB_SRCS})
target_link_libraries(  jsmokeplasma 
                            ${QT_QTCORE_LIBRARY} 
                            ${QT_QTGUI_LIBRARY} 
                            ${QT_QTSCRIPT_LIBRARY} 
                            ${SMOKE_QTCORE_LIBRARY} 
                            smokeplasma 
                            jsmokeruntime )

set_target_properties(jsmokeplasma PROPERTIES PREFIX "")

install(TARGETS jsmokeplasma DESTINATION ${QT_PLUGINS_DIR}/script )
