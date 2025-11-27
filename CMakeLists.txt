cmake_minimum_required(VERSION 3.16)

set(CMAKE_TOOLCHAIN_FILE "${CMAKE_SOURCE_DIR}/sys/cmake/toolchain.cmake")
project(Hakkun-Example VERSION 1.0 LANGUAGES ASM C CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(config/config.cmake)
include(ExternalProject)
ExternalProject_Add(orion
    SOURCE_DIR ${CMAKE_SOURCE_DIR}/include/orion
    CMAKE_ARGS -DCMAKE_BUILD_TYPE=Release
    INSTALL_COMMAND ""
    BINARY_DIR ${CMAKE_BINARY_DIR}/schemas
    BUILD_ALWAYS 1
)

file(GLOB_RECURSE SOURCES_ASM ${PROJECT_SOURCE_DIR}/src/*.S)
file(GLOB_RECURSE SOURCES_C ${PROJECT_SOURCE_DIR}/src/*.c)
file(GLOB_RECURSE SOURCES_CXX ${PROJECT_SOURCE_DIR}/src/*.cpp)
set(SOURCES ${SOURCES_ASM} ${SOURCES_C} ${SOURCES_CXX})
add_executable(${PROJECT_NAME} ${SOURCES})
add_dependencies(${PROJECT_NAME} orion)
set(ROOTDIR ${CMAKE_CURRENT_SOURCE_DIR}/)
include(sys/cmake/bin2s.cmake)
embed_file(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/data/maple_mono.hkf maple_mono_hkf 0x1000 TRUE)
include_directories(include)
include_directories(lib/nnheaders/include)
include_directories(lib/nlohmann)
include_directories(lib/flatbuffers/include)
include_directories(${CMAKE_BINARY_DIR}/schemas/)

include(sys/cmake/module.cmake)
include(sys/cmake/deploy.cmake)
include(sys/cmake/sail.cmake)
usesail(${PROJECT_NAME})