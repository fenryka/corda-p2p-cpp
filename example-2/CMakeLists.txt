link_directories (${CMAKE_SOURCE_DIR}/lib/src)
include_directories (${CMAKE_SOURCE_DIR}/lib/include)

message(STATUS "SRC_DIR = ${CMAKE_SOURCE_DIR}")

set (producer-exe producer2)
set (consumer-exe consumer2)

set (producer-sources producer.cxx)
set (consumer-sources consumer.cxx)

add_executable (${producer-exe} ${producer-sources})

target_include_directories(${producer-exe} PUBLIC ${KAFKA_INCLUDE_DIRS})
target_link_libraries (${producer-exe} ${KAFKA_LIBRARIES} corda-p2p)

# Basically linux, cmake really should add this
if (UNIX AND NOT APPLE)
    # Need to link against the bsd lib to get arc4random
    target_link_libraries (${producer-exe} -lbsd)
endif()

add_executable (${consumer-exe} ${consumer-sources})

target_include_directories(${consumer-exe} PUBLIC ${KAFKA_INCLUDE_DIRS})
target_link_libraries (${consumer-exe} ${KAFKA_LIBRARIES} corda-p2p)
