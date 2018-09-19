set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -include ${CMAKE_CURRENT_SOURCE_DIR}/platform/qt/src/compat.h")

set(BOOST_DIR $ENV{BOOST_DIR})
if(NOT BOOST_DIR)
    message(FATAL_ERROR "BOOST_DIR env variable is not set")
endif()

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_CURRENT_SOURCE_DIR}/platform/default
    ${BOOST_DIR}
)
