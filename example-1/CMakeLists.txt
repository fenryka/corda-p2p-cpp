
pkg_check_modules(GLIB REQUIRED glib-2.0)

set (producer-exe producer)
set (consumer-exe consumer)

set (producer-sources
        common.cxx
        producer.cxx)

set (consumer-sources
        common.cxx
        consumer.cxx)

message(STATUS "KAFKA_LIBRARIES = ${KAFKA_LIBRARIES}")

add_executable (${producer-exe} ${producer-sources})

target_compile_features(${producer-exe} PUBLIC cxx_std_17)
target_compile_features(${producer-exe} PUBLIC c_std_99)

target_include_directories(${producer-exe} PUBLIC ${GLIB_INCLUDE_DIRS} ${KAFKA_INCLUDE_DIRS})
target_link_libraries (${producer-exe} ${KAFKA_LIBRARIES} ${GLIB_LIBRARIES})

# Basically linux, cmake really should add this
if (UNIX AND NOT APPLE)
    # Need to link against the bsd lib to get arc4random
    target_link_libraries (${producer-exe} -lbsd)
endif()

add_executable (${consumer-exe} ${consumer-sources})

target_compile_features(${consumer-exe} PUBLIC cxx_std_17)
target_compile_features(${consumer-exe} PUBLIC c_std_99)

target_include_directories(${consumer-exe} PUBLIC ${GLIB_INCLUDE_DIRS} ${KAFKA_INCLUDE_DIRS})
target_link_libraries (${consumer-exe} ${KAFKA_LIBRARIES} ${GLIB_LIBRARIES})
