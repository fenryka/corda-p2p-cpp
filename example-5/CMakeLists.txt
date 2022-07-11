link_directories (${CMAKE_SOURCE_DIR}/lib/src)
include_directories (${CMAKE_SOURCE_DIR}/lib/include)

message(STATUS "SRC_DIR = ${CMAKE_SOURCE_DIR}")

set (exe json-message-writer)

set (sources main.cxx)

add_executable (${exe} ${sources})

target_include_directories(${exe} PUBLIC ${KAFKA_INCLUDE_DIRS})
target_link_libraries (${exe} ${KAFKA_LIBRARIES} corda-p2p)

set_directory_properties(
        PROPERTIES
        ADDITIONAL_MAKE_CLEAN_FILES "${CMAKE_CURRENT_SOURCE_DIR}/*.json"
        )

