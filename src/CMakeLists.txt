add_definitions(-DBUILD_JSMOKE)

include_directories(${QT_INCLUDES} ${SMOKE_INCLUDE_DIR} ${CMAKE_CURRENT_BINARY_DIR})

set(    jsmokeruntime_LIB_SRCS 
            global.cpp 
            metaobject.cpp
            object.cpp
            smokebinding.cpp  
            utils.cpp
            methodcall.cpp
            methodreturnvalue.cpp
            virtualmethodcall.cpp
            virtualmethodreturnvalue.cpp
            handlers.cpp )

add_library(jsmokeruntime SHARED ${jsmokeruntime_LIB_SRCS})

target_link_libraries(jsmokeruntime ${QT_QTCORE_LIBRARY} ${QT_QTSCRIPT_LIBRARY} ${SMOKE_QTCORE_LIBRARY})
set_target_properties(jsmokeruntime PROPERTIES VERSION 1.0.0 SOVERSION 1)

install(    TARGETS jsmokeruntime 
            DESTINATION ${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX} )

install(    FILES jsmoke_export.h global.h marshall.h marshallmacros.h metaobject.h object.h smokebinding.h 
            DESTINATION ${CMAKE_INSTALL_PREFIX}/include/jsmoke )
