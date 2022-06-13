set (libName corda-p2p)

set (source-files
        config.cxx
        )

ADD_LIBRARY (${libName} SHARED ${source-files})

SET_TARGET_PROPERTIES (${libName} PROPERTIES VERSION ${PROJECT_VERSION})
SET_TARGET_PROPERTIES (${libName} PROPERTIES SOVERSION 1)
SET_TARGET_PROPERTIES (${libName} PROPERTIES PUBLIC_HEADER include/config.h)

message(STATUS "KAFKA_LIBRARIES = ${KAFKA_LIBRARIES}")
target_include_directories(${libName} PUBLIC ${KAFKA_INCLUDE_DIRS})

target_link_libraries (${libName} ${KAFKA_LIBRARIES})

