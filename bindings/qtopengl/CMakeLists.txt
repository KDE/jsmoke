include_directories(${CMAKE_SOURCE_DIR}/src ${QT_INCLUDES})

set(jsmokeqtopengl_LIB_SRCS plugin.cpp qtopengl.cpp qtopenglhandlers.cpp)

add_library(jsmokeqtopengl MODULE ${jsmokeqtopengl_LIB_SRCS})
target_link_libraries(  jsmokeqtopengl 
                            ${QT_QTCORE_LIBRARY} 
                            ${QT_QTGUI_LIBRARY} 
                            ${QT_QTOPENGL_LIBRARY} 
                            ${QT_QTSCRIPT_LIBRARY} 
                            smokeqtcore 
                            smokeqtgui 
                            smokeqtopengl 
                            jsmokeruntime )

set_target_properties(jsmokeqtopengl PROPERTIES PREFIX "")

install(TARGETS jsmokeqtopengl DESTINATION ${QT_PLUGINS_DIR}/script )

