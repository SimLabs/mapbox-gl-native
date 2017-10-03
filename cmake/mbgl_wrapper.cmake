set(MBGL_WRAPPER_SRC_DIR src/mbgl_wrapper)

add_library(mbgl_wrapper SHARED
    include/mbgl_wrapper/mbgl_wrapper.h

    src/mbgl_wrapper/backend.h
    src/mbgl_wrapper/frontend.cpp
    src/mbgl_wrapper/frontend.h
    src/mbgl_wrapper/wrapper.cpp

    platform/${MBGL_PLATFORM}/src/mbgl_wrapper/backend.cpp

    platform/${MBGL_PLATFORM}/src/mbgl_wrapper/file_info.h
)

target_include_directories(mbgl_wrapper
    PRIVATE src
    PRIVATE src/mbgl_wrapper
    PRIVATE platform/${MBGL_PLATFORM}/src/mbgl_wrapper
)

target_link_libraries(mbgl_wrapper
    PRIVATE mbgl-core
    PRIVATE Qt5::Core
    PRIVATE Qt5::Gui
    PRIVATE Qt5::Widgets
    PRIVATE Qt5::OpenGL
)



