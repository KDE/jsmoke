project(jsmokeapi)

find_package(Qt4 REQUIRED)
include_directories(${QT_INCLUDES} ${SMOKE_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

add_executable(jsmokeapi main.cpp)
target_link_libraries(jsmokeapi smokeqtcore ${QT_QTCORE_LIBRARY})
install(    TARGETS jsmokeapi 
            DESTINATION ${CMAKE_INSTALL_PREFIX}/bin )
