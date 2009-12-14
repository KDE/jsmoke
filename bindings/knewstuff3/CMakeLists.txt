include_directories(${CMAKE_SOURCE_DIR}/src ${QT_INCLUDES} ${SMOKE_INCLUDE_DIR})

set(jsmokeknewstuff3_LIB_SRCS plugin.cpp knewstuff3.cpp knewstuff3handlers.cpp)

add_library(jsmokeknewstuff3 MODULE ${jsmokeknewstuff3_LIB_SRCS})
target_link_libraries(  jsmokeknewstuff3 
                            ${QT_QTCORE_LIBRARY} 
                            ${QT_QTGUI_LIBRARY} 
                            ${QT_QTSCRIPT_LIBRARY} 
                            ${SMOKE_QTCORE_LIBRARY} 
                            smokeknewstuff3 
                            jsmokeruntime )

set_target_properties(jsmokeknewstuff3 PROPERTIES PREFIX "")

install(TARGETS jsmokeknewstuff3 DESTINATION ${QT_PLUGINS_DIR}/script )
