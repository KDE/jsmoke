include_directories(${CMAKE_SOURCE_DIR}/src ${QT_INCLUDES} ${SMOKE_INCLUDE_DIR})

set(jsmokesolid_LIB_SRCS plugin.cpp solid.cpp solidhandlers.cpp)

add_library(jsmokesolid MODULE ${jsmokesolid_LIB_SRCS})
target_link_libraries(  jsmokesolid 
                            ${QT_QTCORE_LIBRARY} 
                            ${QT_QTSCRIPT_LIBRARY} 
                            ${SMOKE_QTCORE_LIBRARY} 
                            ${KDE4_SOLID_LIBS} 
                            smokesolid 
                            jsmokeruntime )

set_target_properties(jsmokesolid PROPERTIES PREFIX "")

install(TARGETS jsmokesolid DESTINATION ${QT_PLUGINS_DIR}/script )
