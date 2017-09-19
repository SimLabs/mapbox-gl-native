#
# mbgl-wrapper
#


add_library(mbgl-wrapper SHARED
    platform/windows/src/mbgl_wrapper/mbgl_wrapper.h
    platform/windows/src/mbgl_wrapper/mbgl_wrapper_functions.h
    platform/windows/src/mbgl_wrapper/mbgl_wrapper.cpp
    platform/windows/src/mbgl_wrapper/logging_to_func.cpp
    # platform/windows/src/mbgl_wrapper/logging_to_func.h
    "${PROJECT_BINARY_DIR}/mbgl-wrapper_export.h"
)

target_include_directories(mbgl-wrapper
    PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(mbgl-wrapper
    PRIVATE mbgl-core
    PRIVATE qmapboxgl
)

include(GenerateExportHeader)
generate_export_header(mbgl-wrapper)

add_executable(mbgl-wrapper-test
    platform/windows/src/main.cpp
)

target_link_libraries(mbgl-wrapper-test
    PRIVATE mbgl-core
    PRIVATE qmapboxgl
    PRIVATE mbgl-wrapper
)

target_include_directories(mbgl-wrapper-test
    PRIVATE platform/src/
)