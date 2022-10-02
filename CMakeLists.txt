cmake_minimum_required(VERSION 3.14)

# Want to be able to use the new method of setting the project version
CMAKE_POLICY (SET CMP0048 NEW)

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
# Secondly, looks like Homebrew now sticks things in /opt/homebrew
# instread of /user/local so that also needs adding to the relevent
# dir locations
#
if(APPLE)
    include_directories (
        /usr/local/include
        /opt/homebrew/include)

    link_directories (
        /usr/local/lib
        /opt/homebrew/lib)
endif()


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(PkgConfig REQUIRED)

pkg_check_modules(KAFKA REQUIRED rdkafka++)

ADD_SUBDIRECTORY(example-1)
ADD_SUBDIRECTORY(example-2)
ADD_SUBDIRECTORY(example-3)
ADD_SUBDIRECTORY(example-4)
ADD_SUBDIRECTORY(example-5)

ADD_SUBDIRECTORY(lib)
