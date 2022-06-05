link_directories (${CORDA-P2P-CPP_BINARY_DIR}/lib/src)
include_directories (${CORDA-P2P-CPP_BINARY_DIR}/lib/include)

set (producer-exe producer2)
set (consumer-exe consumer2)

set (producer-sources producer.cxx)
set (consumer-sources consumer.cxx)

add_executable (${producer-exe} ${producer-sources})

target_compile_features(${producer-exe} PUBLIC cxx_std_17)

target_include_directories(${producer-exe} PUBLIC ${KAFKA_INCLUDE_DIRS})
target_link_libraries (${producer-exe} ${KAFKA_LIBRARIES} corda-p2p)

# Basically linux, cmake really should add this
if (UNIX AND NOT APPLE)
    # Need to link against the bsd lib to get arc4random
    target_link_libraries (${producer-exe} -lbsd)
endif()

add_executable (${consumer-exe} ${consumer-sources})

target_compile_features(${consumer-exe} PUBLIC cxx_std_17)

target_include_directories(${consumer-exe} PUBLIC ${KAFKA_INCLUDE_DIRS})
target_link_libraries (${consumer-exe} ${KAFKA_LIBRARIES} corda-p2p)
