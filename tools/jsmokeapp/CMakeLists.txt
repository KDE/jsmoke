find_library( QT_QTSCRIPTTOOLS_LIBRARY
                NAMES QtScriptTools
                PATHS ${QT_LIBRARY_DIR} )
find_path( QT_SCRIPTTOOLS_INCLUDE_DIR
           NAMES qscriptenginedebugger.h
           PATHS ${QT_INCLUDE_DIR}/QtScriptTools )
include_directories(${QT_INCLUDES} ${QT_SCRIPTTOOLS_INCLUDE_DIR})

message( **** ${QT_QTSCRIPTTOOLS_LIBRARY} )

set(jsmokeapp_SRCS src/jsmokeapp.cpp src/common.cpp)
set(jsmokecoreapp_SRCS src/jsmokecoreapp.cpp src/common.cpp)
set(jsmokekapp_SRCS src/jsmokekapp.cpp src/common.cpp)

add_executable(jsmokeapp ${jsmokeapp_SRCS})
add_executable(jsmokecoreapp ${jsmokecoreapp_SRCS})
add_executable(jsmokekapp ${jsmokekapp_SRCS})

target_link_libraries(  jsmokeapp 
                            ${QT_QTCORE_LIBRARY} 
                            ${QT_QTGUI_LIBRARY} 
                            ${QT_QTSCRIPT_LIBRARY}
                            ${QT_QTSCRIPTTOOLS_LIBRARY} )

target_link_libraries(  jsmokecoreapp 
                            ${QT_QTCORE_LIBRARY} 
                            ${QT_QTSCRIPT_LIBRARY} 
                            ${QT_QTSCRIPTTOOLS_LIBRARY} )

target_link_libraries(  jsmokekapp 
                            ${QT_QTCORE_LIBRARY} 
                            ${QT_QTGUI_LIBRARY} 
                            ${QT_QTSCRIPT_LIBRARY}
                            ${QT_QTSCRIPTTOOLS_LIBRARY}
                            ${KDE4_KDECORE_LIBS}
                            ${KDE4_KDEUI_LIBS} )

install(    TARGETS jsmokeapp 
            DESTINATION ${CMAKE_INSTALL_PREFIX}/bin )

install(    TARGETS jsmokecoreapp 
            DESTINATION ${CMAKE_INSTALL_PREFIX}/bin )

install(    TARGETS jsmokekapp 
            DESTINATION ${CMAKE_INSTALL_PREFIX}/bin )


