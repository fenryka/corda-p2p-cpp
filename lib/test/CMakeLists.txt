set (EXE "corda-p2p-test")

link_directories (${CORDA-P2P-CPP_BINARY_DIR}/lib/src)

set (sources
        main.cxx
        json.cxx
        )

set (libs
        corda-p2p
        gtest)

# gtest requires linking against the posix threading libs on linux... but not on
# apple
IF (UNIX AND NOT APPLE)
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)
    set (THREAD_LIB Threads::Threads)
ENDIF()

add_executable (${EXE} ${sources})

target_link_libraries (${EXE} ${libs} ${THREAD_LIB})