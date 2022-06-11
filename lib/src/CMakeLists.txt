include(FetchContent)

set (libname corda-p2p)

set (source-files
        config.cxx
        )

ADD_LIBRARY (${libname} SHARED ${source-files})

#
# Where Corda keeps the P2P avro specs. This URL should always be the latest version of the API, we can worry about
# pinning to a branch/tag later
#
set (p2p-avro https://raw.githubusercontent.com/corda/corda-api/release/os/5.0/data/avro-schema/src/main/resources/avro/net/corda/data)
set (p2p-app-avro ${p2p-avro}/p2p/app)
set (p2p-identity-avro ${p2p-avro}/identity)

function(fetchAvro arg1 arg2 arg3)
    FetchContent_Declare (${arg1}
            DOWNLOAD_COMMAND URL "${arg3}/${arg2}"
            DOWNLOAD_NO_EXTRACT ON
            DOWNLOAD_NAME ${arg2}
            )

    FetchContent_MakeAvailable (${arg1})

    # vexingly, by doing this in a function to reduce the boilerplate the variables we
    # care about aren't exported beyond the scope of the function so set the file
    # name in our parents scope
    set (${arg1}_file ${${arg1}_SOURCE_DIR}/${arg2} PARENT_SCOPE)
endfunction()



fetchAvro (app-message-avro "AppMessage.avsc" ${p2p-app-avro})
fetchAvro (auth-message-avro "AuthenticatedMessage.avsc" ${p2p-app-avro})
fetchAvro (auth-message-header-avro "AuthenticatedMessageHeader.avsc" ${p2p-app-avro})
fetchAvro (unauth-message-avro "UnauthenticatedMessage.avsc" ${p2p-app-avro})
fetchAvro (unauth-message-header-avro "UnauthenticatedMessageHeader.avsc" ${p2p-app-avro})
fetchAvro (holding-identity-avro "HoldingIdentity.avsc" ${p2p-identity-avro})

#
# We need to combine the avro schemas into a single file for the CPP generator to work
#
add_custom_command (OUTPUT avro.asvc
        COMMAND python3 ${CMAKE_CURRENT_SOURCE_DIR}/avro-gen.py avro.asvc ${holding-identity-avro_file} ${unauth-message-header-avro_file} ${auth-message-header-avro_file} ${unauth-message-avro_file} ${auth-message-avro_file} ${app-message-avro_file}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

add_custom_command(OUTPUT avro.h
        COMMAND  avrogencpp -i avro.asvc -o avro.h
        )

add_custom_target(
        buildAvro ALL
        DEPENDS avro.h
)

target_include_directories(${libname} PUBLIC ${GLIB_INCLUDE_DIRS} ${KAFKA_INCLUDE_DIRS})
target_link_libraries (${libname} ${KAFKA_LIBRARIES} ${GLIB_LIBRARIES})

SET_TARGET_PROPERTIES (${libName} PROPERTIES VERSION ${PROJECT_VERSION})
SET_TARGET_PROPERTIES (${libName} PROPERTIES SOVERSION 1)
SET_TARGET_PROPERTIES (${libName} PROPERTIES PUBLIC_HEADER include/config.h)
