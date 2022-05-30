
# Want to be able to use the new method of setting the project version
CMAKE_POLICY (SET CMP0048 NEW)

CMAKE_MINIMUM_REQUIRED (VERSION 3.10)

PROJECT (CORDA-P2P-CPP VERSION 0.0.1)

#ADD_DEFINITIONS ("-Wall -g -ansi -pedantic")

#
# Set this to true to help debug any issues with cmake
#
set (CMAKE_VERBOSE_MAKEFILE "FALSE" CACHE BOOL "" FORCE)

#
# For "reasons" something to do with the way the command line compiler
# is invoked on MacOS /usr/local/include is removed from the 
# system include path so add it back in or we will fail to find
# qpid-python files as brew installs it there
#
if(APPLE)
    include_directories (/usr/local/include)
    link_directories (/usr/local/lib)
endif()

#
# If you need to have openssl@1.1 first in your PATH run:
# echo 'export PATH="/usr/local/opt/openssl@1.1/bin:$PATH"' >> ~/.zshrc
#
# For compilers to find openssl@1.1 you may need to set:
# export LDFLAGS="-L/usr/local/opt/openssl@1.1/lib"
# export CPPFLAGS="-I/usr/local/opt/openssl@1.1/include"
#
# For pkg-config to find openssl@1.1 you may need to set:
# export PKG_CONFIG_PATH="/usr/local/opt/openssl@1.1/lib/pkgconfig
#

find_package(PkgConfig REQUIRED)

pkg_check_modules(KAFKA REQUIRED rdkafka++)
pkg_check_modules(GLIB REQUIRED glib-2.0)


set (producer-exe producer)
set (consumer-exe consumer)

set (producer-sources
        common.cxx
        producer.cxx)

set (consumer-sources
        common.cxx
        consumer.cxx)

add_executable (${producer-exe} ${producer-sources})

target_compile_features(${producer-exe} PUBLIC cxx_std_17)
target_compile_features(${producer-exe} PUBLIC c_std_99)

target_include_directories(${producer-exe} PUBLIC ${GLIB_INCLUDE_DIRS} ${KAFKA_INCLUDE_DIRS})
target_link_libraries (${producer-exe} ${KAFKA_LIBRARIES} ${GLIB_LIBRARIES})

add_executable (${consumer-exe} ${consumer-sources})

target_compile_features(${consumer-exe} PUBLIC cxx_std_17)
target_compile_features(${consumer-exe} PUBLIC c_std_99)

target_include_directories(${consumer-exe} PUBLIC ${GLIB_INCLUDE_DIRS} ${KAFKA_INCLUDE_DIRS})
target_link_libraries (${consumer-exe} ${KAFKA_LIBRARIES} ${GLIB_LIBRARIES})
