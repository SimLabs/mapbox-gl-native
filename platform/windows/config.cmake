mason_use(nunicode VERSION 1.7.1)

# Link with libuv. This is not part of loop-uv.cmake because loop-uv.cmake is also
# used by node.cmake, where we want to link with the libuv provided by node itself.
target_link_libraries(mbgl-loop-uv
    PUBLIC -luv
)

macro(mbgl_platform_core)
    message("Building core for windows")

    target_compile_options(mbgl-core
        PRIVATE -Wa,-mbig-obj
        PRIVATE -mwindows
    )

    target_link_libraries(mbgl-core
        PUBLIC -lgdi32
        PUBLIC -lkernel32
        PUBLIC -lopengl32
        PUBLIC -lsqlite3
        PUBLIC -lpng
        PUBLIC -lwebp
        PUBLIC -lz
        PUBLIC -lcurl
        PUBLIC -ljpeg
        PUBLIC -lsicudt
        PUBLIC -lsicudtd
        PUBLIC -lsicuin
        PUBLIC -lsicuind
        PUBLIC -lsicuio
        PUBLIC -lsicuiod
        PUBLIC -lsicutest
        PUBLIC -lsicutestd
        PUBLIC -lsicutu
        PUBLIC -lsicutud
        PUBLIC -lsicuuc
        PUBLIC -lsicuucd
    )

    target_add_mason_package(mbgl-core PUBLIC nunicode)
    
    target_sources(mbgl-core
        PRIVATE platform/windows/src/headless_backend_wgl.cpp
        PRIVATE platform/windows/src/headless_display_wgl.cpp
    )

    target_sources(mbgl-core
        # File source
        PRIVATE platform/default/asset_file_source.cpp
        PRIVATE platform/default/default_file_source.cpp
        PRIVATE platform/default/local_file_source.cpp
        PRIVATE platform/default/http_file_source.cpp
        PRIVATE platform/default/online_file_source.cpp

        # Offline
        PRIVATE platform/default/mbgl/storage/offline.cpp
        PRIVATE platform/default/mbgl/storage/offline_database.cpp
        PRIVATE platform/default/mbgl/storage/offline_database.hpp
        PRIVATE platform/default/mbgl/storage/offline_download.cpp
        PRIVATE platform/default/mbgl/storage/offline_download.hpp
        PRIVATE platform/default/sqlite3.cpp
        PRIVATE platform/default/sqlite3.hpp

        # Misc
        PRIVATE platform/default/logging_stderr.cpp
        PRIVATE platform/default/string_stdlib.cpp
        PRIVATE platform/default/thread.cpp
        PRIVATE platform/default/bidi.cpp
        PRIVATE platform/default/thread_local.cpp
        PRIVATE platform/default/run_loop.cpp
        PRIVATE platform/default/utf.cpp

        # Image handling
        PRIVATE platform/default/image.cpp
        PRIVATE platform/default/jpeg_reader.cpp
        PRIVATE platform/default/png_writer.cpp
        PRIVATE platform/default/png_reader.cpp
        PRIVATE platform/default/webp_reader.cpp

        # Headless view
        PRIVATE platform/default/mbgl/gl/headless_frontend.cpp
        PRIVATE platform/default/mbgl/gl/headless_frontend.hpp
        PRIVATE platform/default/mbgl/gl/headless_backend.cpp
        PRIVATE platform/default/mbgl/gl/headless_backend.hpp
        PRIVATE platform/default/mbgl/gl/headless_display.hpp

        # Thread pool
        PRIVATE platform/default/mbgl/util/default_thread_pool.hpp
        PRIVATE platform/default/mbgl/util/default_thread_pool.cpp
        PRIVATE platform/default/mbgl/util/shared_thread_pool.cpp
        PRIVATE platform/default/mbgl/util/shared_thread_pool.hpp
    )

    target_include_directories(mbgl-core
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/platform/default
    )
endmacro()

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/platform/default
)

# TODO : resolve problem with unlinked interface D:
add_library(mapbox-interface SHARED
    platform/windows/src/verbose_map_observer.hpp
    platform/windows/src/interface.cpp
    platform/windows/src/interface.h
    "${PROJECT_BINARY_DIR}/mapbox-interface_export.h"
)

target_link_libraries(mapbox-interface
    PRIVATE mbgl-core
    PRIVATE mbgl-loop-uv
    PRIVATE -lopengl32
    PRIVATE -lglew32
)

include(GenerateExportHeader)
generate_export_header(mapbox-interface)

add_executable(mapbox-interface-test
    ${CMAKE_CURRENT_SOURCE_DIR}/platform/default/mbgl/gl/headless_display.hpp
    ${CMAKE_CURRENT_SOURCE_DIR}/platform/windows/src/headless_display_wgl.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/platform/windows/src/interface.h
    ${CMAKE_CURRENT_SOURCE_DIR}/platform/windows/src/main.cpp
)

target_link_libraries(mapbox-interface-test
    PRIVATE mapbox-interface
)

add_custom_command(TARGET mapbox-interface-test POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_FILE:mapbox-interface> $<TARGET_FILE_DIR:mapbox-interface-test>
    COMMENT "Copying mapbox-interface shared lib alongside mapbox-interface-test."
    VERBATIM
)

macro(mbgl_platform_render)
    target_link_libraries(mbgl-render
        PUBLIC -lboost
        PUBLIC -lboost_program_options-mt
        PRIVATE mbgl-loop-uv
    )
endmacro()