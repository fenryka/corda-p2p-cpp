include(FetchContent)

set (libname corda-p2p)

set (source-files
        config.cxx
        avro.cxx
        identity.cxx
        X500.cxx
        Message.cxx
        MessageFactory.cxx
        SchemaBuilder.cxx)

#
# Where Corda keeps the P2P avro specs. This URL should always be the latest version of the API, we can worry about
# pinning to a branch/tag later
#
set (p2p-avro https://raw.githubusercontent.com/corda/corda-api/release/os/5.0/data/avro-schema/src/main/resources/avro/net/corda/data)
set (p2p-app-avro ${p2p-avro}/p2p/app)
set (p2p-identity-avro ${p2p-avro}/identity)

# function to pull down each dependency from the Corda API repo, basically just saves the
# the replication of a lot of boiler plate
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

#
# Actually fetch the files
#
fetchAvro (app-message-avro "AppMessage.avsc" ${p2p-app-avro})
fetchAvro (auth-message-avro "AuthenticatedMessage.avsc" ${p2p-app-avro})
fetchAvro (auth-message-header-avro "AuthenticatedMessageHeader.avsc" ${p2p-app-avro})
fetchAvro (unauth-message-avro "UnauthenticatedMessage.avsc" ${p2p-app-avro})
fetchAvro (unauth-message-header-avro "UnauthenticatedMessageHeader.avsc" ${p2p-app-avro})
fetchAvro (holding-identity-avro "HoldingIdentity.avsc" ${p2p-identity-avro})

set (avro-files
        ${holding-identity-avro_file}
        ${unauth-message-header-avro_file}
        ${auth-message-header-avro_file}
        ${unauth-message-avro_file}
        ${auth-message-avro_file}
        ${app-message-avro_file}
)

#
# We need to combine the avro schemas into a single file for the CPP generator to work
#
add_custom_command (
        OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/avro.asvc
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/avro-gen.py
        COMMAND python3 ${CMAKE_CURRENT_SOURCE_DIR}/avro-gen.py ${CMAKE_CURRENT_SOURCE_DIR}/avro.asvc ${avro-files}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT
            "Merging Corda AVRO schemas for cpp-gen"
        )

add_custom_command (
        OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/avro.schema
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/schema-gen.py
        COMMAND python3 ${CMAKE_CURRENT_SOURCE_DIR}/schema-gen.py ${CMAKE_CURRENT_SOURCE_DIR}/avro.schema ${holding-identity-avro_file} ${unauth-message-header-avro_file} ${auth-message-header-avro_file} ${unauth-message-avro_file} ${auth-message-avro_file} ${app-message-avro_file}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        COMMENT
            "Merging Corda AVRO schemas for json schema"
)

add_custom_command(
        OUTPUT avro.h
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/avro.asvc
        COMMAND avrogencpp -n corda_p2p -i ${CMAKE_CURRENT_SOURCE_DIR}/avro.asvc -o avro.h
        COMMENT
            "Building avro header"
)

add_custom_target (ASCV ALL
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/avro.asvc
)

add_custom_target (AVRO-SCHEMA ALL
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/avro.schema
        COMMENT "Satisfy AVRO Jason Schema gen dependency"
)

ADD_LIBRARY (${libname} SHARED ${source-files} avro.h)

target_include_directories(${libname} PUBLIC ${KAFKA_INCLUDE_DIRS})
target_link_libraries (${libname} ${KAFKA_LIBRARIES} avrocpp)

SET_TARGET_PROPERTIES (${libname} PROPERTIES VERSION ${PROJECT_VERSION})
SET_TARGET_PROPERTIES (${libname} PROPERTIES SOVERSION 1)
SET_TARGET_PROPERTIES (${libname} PROPERTIES PUBLIC_HEADER include/config.h)

message(STATUS "KAFKA_LIBRARIES = ${KAFKA_LIBRARIES}")
target_include_directories(${libname} PUBLIC ${KAFKA_INCLUDE_DIRS})

target_link_libraries (${libname} ${KAFKA_LIBRARIES})

