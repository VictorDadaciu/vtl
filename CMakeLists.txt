cmake_minimum_required(VERSION 3.21..4.4)

project(
    vtl
    VERSION 0.0.1
    DESCRIPTION "very test library"
    LANGUAGES C CXX
)

include(GNUInstallDirs)

if (PROJECT_IS_TOP_LEVEL)
    message("vtl is the top-level project")
    set(VTL_IS_TOP_LEVEL ON)
endif()

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

add_library(vtl STATIC
    include/vtl/suite.h
    src/suite.cpp
)

set_property(TARGET vtl
    PROPERTY CXX_STANDARD 26
)

target_compile_options(vtl
    INTERFACE
        -std=c++26
        -freflection
)

target_include_directories(vtl
    PRIVATE
        include/vtl
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/vtl>
)

option(VTL_INSTALL "Install vtl" ${VTL_IS_TOP_LEVEL})
if (${VTL_INSTALL})
    message("Configuring vtl install...")
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/cmake/vtlConfig.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/vtlConfig.cmake
        COPYONLY
    )
    install(TARGETS vtl
        EXPORT vtlTargets
    )
    install(EXPORT vtlTargets
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/vtl/cmake
        NAMESPACE vtl::
    )
    install(FILES "${CMAKE_CURRENT_BINARY_DIR}/vtlConfig.cmake"
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/vtl/cmake
    )
    install(DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/include/vtl"
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/vtl
    )
    export(TARGETS vtl
        NAMESPACE vtl::
        FILE vtlTargets.cmake
    )
endif()

if (${VTL_IS_TOP_LEVEL})
    message("Installing vtl...")
    add_custom_target(
        vtl_forced_install ALL
        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/__dummy_file
    )
    add_custom_command(
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/__dummy_file
        COMMENT "Installing vtl to ~/.local"
        COMMAND ./do_install.sh
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
    add_dependencies(vtl_forced_install vtl)
endif()

