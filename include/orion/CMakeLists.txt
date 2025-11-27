cmake_minimum_required(VERSION 3.16)
project(orion)

# TODO: should flatbuffers be in orion too?
add_subdirectory(${CMAKE_SOURCE_DIR}/../../lib/flatbuffers flatbuffers_build)

set(FLATC_FLAGS --gen-mutable --reflect-names --reflect-types --filename-suffix _flatbuffer)
file(GLOB_RECURSE ALL_SCHEMAS ${PROJECT_SOURCE_DIR}/schemas/*.fbs)

foreach(SCHEMA ${ALL_SCHEMAS})
    get_filename_component(FOLDER ${SCHEMA} DIRECTORY)
    file(RELATIVE_PATH REL_FOLDER ${PROJECT_SOURCE_DIR}/schemas/ ${FOLDER})
    string(REPLACE "/" "_" REL_FOLDER_SANITIZED ${REL_FOLDER})
    get_filename_component(SCHEMA_NAME_WE ${SCHEMA} NAME_WE)
    flatbuffers_generate_headers(
        TARGET orion_${REL_FOLDER_SANITIZED}
        SCHEMAS ${SCHEMA}
        FLAGS ${FLATC_FLAGS}
    )
    set(HEADER_SRC ${CMAKE_CURRENT_BINARY_DIR}/orion_${REL_FOLDER_SANITIZED}/${SCHEMA_NAME_WE}_flatbuffer.h)
    set(DEST_DIR ${CMAKE_CURRENT_BINARY_DIR}/orion/${REL_FOLDER})
    file(MAKE_DIRECTORY ${DEST_DIR})
    set(HEADER_DST ${DEST_DIR}/${SCHEMA_NAME_WE}_flatbuffer.h)

    add_custom_command(
        OUTPUT ${HEADER_DST}
        COMMAND ${CMAKE_COMMAND} -E copy ${HEADER_SRC} ${HEADER_DST}
        DEPENDS GENERATE_orion_${REL_FOLDER_SANITIZED}
        COMMENT "Copying FlatBuffers header ${SCHEMA_NAME_WE}_flatbuffer.h"
    )

    list(APPEND ALL_COPIED_HEADERS ${HEADER_DST})
endforeach()

add_custom_target(orion_headers ALL DEPENDS ${ALL_COPIED_HEADERS})