set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /FIvs_compat.h /std:c++latest /bigobj")

set(BOOST_DIR $ENV{BOOST_DIR})
if(NOT BOOST_DIR)
    message(FATAL_ERROR "BOOST_DIR env variable is not set")
endif()
message("Boost dir is: " ${BOOST_DIR})

add_definitions(-DGLEW_STATIC)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/platform/default
    ${CMAKE_CURRENT_SOURCE_DIR}/platform/msvc/src
    ${CMAKE_CURRENT_SOURCE_DIR}/platform/msvc/ext/include
    ${BOOST_DIR}
)