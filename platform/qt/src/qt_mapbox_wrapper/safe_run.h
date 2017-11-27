#pragma once

#include "mbgl/util/logging.hpp"

namespace qt_mapbox_wrapper 
{

namespace safe_run_detail
{

inline void return_nothing() {};
    
} // namespace safe_run_detail

} // namespace qt_mapbox_wrapper 



#define SAFE_RUN_BEGIN() \
    try \
    { 

#define SAFE_RUN_END_RET(ret) \
    } \
    catch (std::exception const &e) \
    { \
        mbgl::Log::Error(mbgl::Event::General, "%s failed: %s", BOOST_CURRENT_FUNCTION, e.what()); \
        return ret; \
    } \
    catch (...) \
    { \
        mbgl::Log::Error(mbgl::Event::General, "%s failed", BOOST_CURRENT_FUNCTION); \
        return ret; \
    }


#define SAFE_RUN_END() SAFE_RUN_END_RET(qt_mapbox_wrapper::safe_run_detail::return_nothing())

