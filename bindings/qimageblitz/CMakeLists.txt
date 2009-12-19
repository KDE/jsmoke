include_directories(${CMAKE_SOURCE_DIR}/src ${QT_INCLUDES})

set(jsmokeqimageblitz_LIB_SRCS plugin.cpp qimageblitz.cpp qimageblitzhandlers.cpp)

add_library(jsmokeqimageblitz MODULE ${jsmokeqimageblitz_LIB_SRCS})
target_link_libraries(  jsmokeqimageblitz 
                            ${QT_QTCORE_LIBRARY} 
                            ${QT_QTSCRIPT_LIBRARY} 
                            smokeqtcore 
                            smokeqimageblitz 
                            jsmokeruntime )

set_target_properties(jsmokeqimageblitz PROPERTIES PREFIX "")

install(TARGETS jsmokeqimageblitz DESTINATION ${QT_PLUGINS_DIR}/script )
