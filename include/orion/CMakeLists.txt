cmake_minimum_required(VERSION 3.16)
project(orion)

# TODO: should flatbuffers be in orion too?
add_subdirectory(${CMAKE_SOURCE_DIR}/../../lib/flatbuffers flatbuffers_build)

set(FLATC_FLAGS --gen-mutable --reflect-names --reflect-types --filename-suffix _flatbuffer)
file(GLOB_RECURSE ALL_SCHEMAS ${PROJECT_SOURCE_DIR}/schemas/*.fbs)

foreach(SCHEMA ${ALL_SCHEMAS})
    get_filename_component(FOLDER ${SCHEMA} DIRECTORY)
    file(RELATIVE_PATH REL_FOLDER ${PROJECT_SOURCE_DIR}/schemas/ ${FOLDER})
    flatbuffers_generate_headers(
        TARGET orion
        SCHEMAS ${SCHEMA}
        INCLUDE_PREFIX ${REL_FOLDER}
        FLAGS ${FLATC_FLAGS}
    )
endforeach()
