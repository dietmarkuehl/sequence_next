# SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception

include(GNUInstallDirs)

add_library(beman.sequence_next)
add_library(beman::sequence_next ALIAS beman.sequence_next)

target_sources(beman.sequence_next PRIVATE sequence_next.cpp)

target_sources(
    beman.sequence_next
    PUBLIC
        FILE_SET HEADERS
        BASE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/../../../include
        FILES
            ${CMAKE_CURRENT_SOURCE_DIR}/../../../include/beman/sequence_next/sequence_next.hpp
)

set_target_properties(
    beman.sequence_next
    PROPERTIES VERIFY_INTERFACE_HEADER_SETS ON EXPORT_NAME sequence_next
)

install(
    TARGETS beman.sequence_next COMPONENT beman.sequence_next
    EXPORT beman.sequence_next
    DESTINATION
    ${CMAKE_INSTALL_LIBDIR}$<$<CONFIG:Debug>:/debug>
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}$<$<CONFIG:Debug>:/debug>
    FILE_SET HEADERS DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
)

if(BEMAN_SEQUENCE_NEXT_INSTALL_CONFIG_FILE_PACKAGE)
    include(CMakePackageConfigHelpers)

    configure_package_config_file(
        "${CMAKE_CURRENT_SOURCE_DIR}/beman.sequence_next-config.cmake.in"
        "${CMAKE_CURRENT_BINARY_DIR}/beman.sequence_next-config.cmake"
        INSTALL_DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/beman.sequence_next"
        PATH_VARS PROJECT_NAME PROJECT_VERSION
    )

    write_basic_package_version_file(
        "${CMAKE_CURRENT_BINARY_DIR}/beman.sequence_next-version.cmake"
        VERSION ${PROJECT_VERSION}
        COMPATIBILITY ExactVersion
    )

    install(
        FILES
            "${CMAKE_CURRENT_BINARY_DIR}/beman.sequence_next-config.cmake"
            "${CMAKE_CURRENT_BINARY_DIR}/beman.sequence_next-version.cmake"
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/beman.sequence_next"
        COMPONENT beman.sequence_next
    )

    install(
        EXPORT beman.sequence_next
        DESTINATION "${CMAKE_INSTALL_LIBDIR}/cmake/beman.sequence_next"
        NAMESPACE beman::
        FILE beman.sequence_next-targets.cmake
        COMPONENT beman.sequence_next
    )
endif()
