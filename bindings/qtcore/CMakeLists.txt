include_directories(${CMAKE_SOURCE_DIR}/src ${QT_INCLUDES} ${SMOKE_INCLUDE_DIR})

set(jsmokeqtcore_LIB_SRCS plugin.cpp qtcore.cpp qtcorehandlers.cpp)

add_library(jsmokeqtcore MODULE ${jsmokeqtcore_LIB_SRCS})
target_link_libraries(jsmokeqtcore ${QT_QTCORE_LIBRARY} ${QT_QTSCRIPT_LIBRARY} ${SMOKE_QTCORE_LIBRARY} jsmokeruntime)

set_target_properties(jsmokeqtcore PROPERTIES PREFIX "")

install(TARGETS jsmokeqtcore DESTINATION ${QT_PLUGINS_DIR}/script )


