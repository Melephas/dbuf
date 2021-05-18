cmake_minimum_required(VERSION 3.13)
project(dbuf C)

set(CMAKE_C_STANDARD 11)

add_library(
    dbuf SHARED
    src/libdbuf.c
)

target_include_directories(
    dbuf
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
#    PUBLIC include
)

install(
    TARGETS dbuf
    DESTINATION lib
    EXPORT dbufTargets
)

install(
    FILES "include/libdbuf.h"
    DESTINATION include
)

include(CMakePackageConfigHelpers)

# Generate the config file that includes the exports
configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/Config.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/dbufConfig.cmake"
    INSTALL_DESTINATION "lib/cmake/example"
    NO_SET_AND_CHECK_MACRO
    NO_CHECK_REQUIRED_COMPONENTS_MACRO
)

# Generate the version file for the config file
write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/dbufConfigVersion.cmake"
    VERSION "${dbuf_VERSION_MAJOR}.${dbuf_VERSION_MINOR}"
    COMPATIBILITY AnyNewerVersion
)

install(
    EXPORT dbufTargets
    FILE dbufTargets.cmake
    DESTINATION lib/cmake/dbuf
)