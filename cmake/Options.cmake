option(PROJECT_WARNINGS_AS_ERRORS "Treat warnings as errors" OFF)
option(USE_ASAN "Use Address Sanitizer" OFF)
option(USE_MSAN "Use Memory Sanitizer" OFF)
option(CMAKE_COLOR_DIAGNOSTICS "Enable color diagnostics" ON)
option(BUILD_SHARED_LIBS "Build SFML as shared library" FALSE)

set(DESTINATION_DIR "bin")

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(CMAKE_INSTALL_PREFIX "${PROJECT_SOURCE_DIR}/install_dir" CACHE PATH "..." FORCE)
endif()

if (NOT DEFINED FETCHCONTENT_BASE_DIR AND DEFINED ENV{FETCHCONTENT_BASE_DIR})
    set(FETCHCONTENT_BASE_DIR $ENV{FETCHCONTENT_BASE_DIR}
        CACHE INTERNAL "Override deps directory via environment variable"
        FORCE
    )
endif()

if (NOT DEFINED FETCHCONTENT_SOURCE_DIR_SFML AND DEFINED ENV{SFML3_DIR})
    set(FETCHCONTENT_SOURCE_DIR_SFML $ENV{SFML3_DIR}
        CACHE INTERNAL "Override source directory for SFML3 via environment variable"
        FORCE
    )
endif()
#########

set(is_debug "$<CONFIG:Debug>")
set(is_rel_with_deb "$<CONFIG:RelWithDebInfo>")
set(debug_mode "$<OR:${is_debug},${is_rel_with_deb}>")
