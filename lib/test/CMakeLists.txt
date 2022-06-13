set (EXE "corda-p2p-test")

link_directories (${CORDA-P2P-CPP_BINARY_DIR}/lib/src)

set (sources
        main.cxx
        json.cxx
        avro.cxx
        X500Support.cxx
        )

set (libs
        corda-p2p
        gtest
        avrocpp
        )

# gtest requires linking against the posix threading libs on linux... but not on
# apple
IF (UNIX AND NOT APPLE)
    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)
    set (THREAD_LIB Threads::Threads)
ENDIF()

add_executable (${EXE} ${sources})
target_compile_features(${EXE} PRIVATE cxx_std_17)

target_link_libraries (${EXE} ${libs} ${THREAD_LIB} ${KAFKA_LIBRARIES})
target_include_directories(${EXE} PUBLIC ${KAFKA_INCLUDE_DIRS})
