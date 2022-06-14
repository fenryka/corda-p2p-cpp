link_directories (${CMAKE_SOURCE_DIR}/lib/src)
include_directories (${CMAKE_SOURCE_DIR}/lib/include)

message(STATUS "SRC_DIR = ${CMAKE_SOURCE_DIR}")

set (exe two-way)

set (two-way-sources main.cxx)

add_executable (${exe} ${two-way-sources})

target_include_directories(${exe} PUBLIC ${KAFKA_INCLUDE_DIRS})
target_link_libraries (${exe} ${KAFKA_LIBRARIES} corda-p2p pthread)

