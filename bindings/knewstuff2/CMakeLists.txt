include_directories(${CMAKE_SOURCE_DIR}/src ${QT_INCLUDES} ${SMOKE_INCLUDE_DIR})

set(jsmokeknewstuff2_LIB_SRCS plugin.cpp knewstuff2.cpp knewstuff2handlers.cpp)

add_library(jsmokeknewstuff2 MODULE ${jsmokeknewstuff2_LIB_SRCS})
target_link_libraries(  jsmokeknewstuff2 
                            ${QT_QTCORE_LIBRARY} 
                            ${QT_QTGUI_LIBRARY} 
                            ${QT_QTSCRIPT_LIBRARY} 
                            ${SMOKE_QTCORE_LIBRARY} 
                            smokeknewstuff2 
                            jsmokeruntime )

set_target_properties(jsmokeknewstuff2 PROPERTIES PREFIX "")

install(TARGETS jsmokeknewstuff2 DESTINATION ${QT_PLUGINS_DIR}/script )
