include_directories(${CMAKE_SOURCE_DIR}/src ${QT_INCLUDES} ${SMOKE_INCLUDE_DIR})

set(jsmokeqtxmlpatterns_LIB_SRCS plugin.cpp qtxmlpatterns.cpp qtxmlpatternshandlers.cpp)

add_library(jsmokeqtxmlpatterns MODULE ${jsmokeqtxmlpatterns_LIB_SRCS})
target_link_libraries(  jsmokeqtxmlpatterns 
                            ${QT_QTCORE_LIBRARY} 
                            ${QT_QTXMLPATTERNS_LIBRARY} 
                            ${QT_QTSCRIPT_LIBRARY} 
                            ${SMOKE_QTCORE_LIBRARY} 
                            smokeqtxmlpatterns 
                            jsmokeruntime )

set_target_properties(jsmokeqtxmlpatterns PROPERTIES PREFIX "")

install(TARGETS jsmokeqtxmlpatterns DESTINATION ${QT_PLUGINS_DIR}/script )

