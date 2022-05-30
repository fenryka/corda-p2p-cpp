set (libname corda-p2p)

set (source-files
        config.cxx
        )

ADD_LIBRARY (${libname} SHARED ${source-files})

target_include_directories(${libname} PUBLIC ${GLIB_INCLUDE_DIRS} ${KAFKA_INCLUDE_DIRS})
target_link_libraries (${libname} ${KAFKA_LIBRARIES} ${GLIB_LIBRARIES})


SET_TARGET_PROPERTIES (${libName} PROPERTIES VERSION ${PROJECT_VERSION})
SET_TARGET_PROPERTIES (${libName} PROPERTIES SOVERSION 1)
SET_TARGET_PROPERTIES (${libName} PROPERTIES PUBLIC_HEADER include/config.h)
