# This file is to be reused by target platforms that don't
# support `mason` (i.e. Yocto). Do not add any `mason` macro.

option(WITH_QT_DECODERS "Use builtin Qt image decoders" OFF)
option(WITH_QT_I18N     "Use builtin Qt i18n support"   OFF)
option(WITH_QT_4        "Use Qt4 instead of Qt5"        OFF)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden -D__QT__")
set(CMAKE_C_FLAGS   "${CMAKE_C_FLAGS}   -fvisibility=hidden -D__QT__")

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

if (MSVC)
    include(platform/qt/qt_msvc.cmake)
endif ()

set(MBGL_QT_FILES
    # File source
    PRIVATE platform/default/asset_file_source.cpp
    PRIVATE platform/default/default_file_source.cpp
    PRIVATE platform/default/local_file_source.cpp
    PRIVATE platform/default/online_file_source.cpp

    # Offline
    PRIVATE platform/default/mbgl/storage/offline.cpp
    PRIVATE platform/default/mbgl/storage/offline_database.cpp
    PRIVATE platform/default/mbgl/storage/offline_database.hpp
    PRIVATE platform/default/mbgl/storage/offline_download.cpp
    PRIVATE platform/default/mbgl/storage/offline_download.hpp
    PRIVATE platform/default/sqlite3.hpp

    # Misc
    PRIVATE platform/default/logging_stderr.cpp

    # Thread pool
    PRIVATE platform/default/mbgl/util/shared_thread_pool.cpp
    PRIVATE platform/default/mbgl/util/shared_thread_pool.hpp
    PRIVATE platform/default/mbgl/util/default_thread_pool.cpp
    PRIVATE platform/default/mbgl/util/default_thread_pool.hpp

    # Render
    PRIVATE platform/default/mbgl/gl/headless_backend.cpp
    PRIVATE platform/default/mbgl/gl/headless_backend.hpp
    PRIVATE platform/default/mbgl/gl/headless_display.cpp
    PRIVATE platform/default/mbgl/gl/headless_display.hpp
    PRIVATE platform/default/mbgl/gl/headless_frontend.cpp
    PRIVATE platform/default/mbgl/gl/headless_frontend.hpp

    # Thread
    PRIVATE platform/qt/src/thread_local.cpp

    # Platform integration
    PRIVATE platform/qt/src/async_task.cpp
    PRIVATE platform/qt/src/async_task_impl.hpp
    PRIVATE platform/qt/src/http_file_source.cpp
    PRIVATE platform/qt/src/http_file_source.hpp
    PRIVATE platform/qt/src/http_request.cpp
    PRIVATE platform/qt/src/http_request.hpp
    PRIVATE platform/qt/src/qt_image.cpp
    PRIVATE platform/qt/src/run_loop.cpp
    PRIVATE platform/qt/src/run_loop_impl.hpp
    PRIVATE platform/qt/src/sqlite3.cpp
    PRIVATE platform/qt/src/string_stdlib.cpp
    PRIVATE platform/qt/src/timer.cpp
    PRIVATE platform/qt/src/timer_impl.hpp
    PRIVATE platform/qt/src/utf.cpp

    PRIVATE platform/qt/src/gl_functions.cpp
    PRIVATE platform/qt/src/headless_backend_impl.cpp
)

include_directories(
    PRIVATE platform/qt/include
)

# Shared library
add_library(qmapboxgl SHARED
    platform/qt/include/qmapbox.hpp
    platform/qt/include/qmapboxgl.hpp
    platform/qt/src/qmapbox.cpp
    platform/qt/src/qmapboxgl.cpp
    platform/qt/src/qmapboxgl_p.hpp
    platform/qt/src/qmapboxgl_renderer_frontend_p.hpp
    platform/qt/src/qmapboxgl_renderer_frontend_p.cpp
    platform/default/mbgl/util/default_styles.hpp
)

if (CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
    if (NOT MSVC)
        target_compile_options(qmapboxgl
            PRIVATE -Wa,-mbig-obj
            PRIVATE -mwindows
        )
    endif (NOT MSVC)
endif()

# C++ app
add_executable(mbgl-qt
    platform/qt/app/main.cpp
    platform/qt/app/mapwindow.cpp
    platform/qt/app/mapwindow.hpp
    platform/qt/resources/common.qrc
)

if (NOT MSVC)
target_link_libraries(mbgl-qt
    PRIVATE -lglew32
)

endif()

xcode_create_scheme(TARGET mbgl-qt)

if(WITH_QT_4)
    include(platform/qt/qt4.cmake)
else()
    include(platform/qt/qt5.cmake)
endif()

# OS specific configurations
if (MASON_PLATFORM STREQUAL "osx" OR MASON_PLATFORM STREQUAL "ios")
    list(APPEND MBGL_QT_FILES
        PRIVATE platform/darwin/src/nsthread.mm
    )
    list(APPEND MBGL_QT_LIBRARIES
        PRIVATE "-framework Foundation"
        PRIVATE "-framework OpenGL"
    )
elseif (CMAKE_HOST_SYSTEM_NAME STREQUAL "Linux")
    list(APPEND MBGL_QT_FILES
        PRIVATE platform/default/thread.cpp
    )
    list(APPEND MBGL_QT_LIBRARIES
        PRIVATE -lGL
    )
elseif (CMAKE_HOST_SYSTEM_NAME STREQUAL "Windows")
    list(APPEND MBGL_QT_FILES
        PRIVATE platform/qt/src/thread.cpp
    )
    list(APPEND MBGL_QT_LIBRARIES
        PRIVATE -lopengl32
        PRIVATE -lglew32
    )

    if (NOT_MSVC)
        add_compile_options(
            -Wa,-mbig-obj
        )
    endif (NOT_MSVC)
endif()

if (NOT ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_CURRENT_BINARY_DIR})
    add_custom_command(
        TARGET qmapboxgl
        POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/platform/qt/include
                ${CMAKE_CURRENT_BINARY_DIR}/platform/qt/include
    )
endif()

xcode_create_scheme(TARGET qmapboxgl)


