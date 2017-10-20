#pragma once

#if defined(qt_mapbox_wrapper_EXPORTS)
#   define QT_MAPBOX_WRAPPER_API __declspec(dllexport)
#else
#   define QT_MAPBOX_WRAPPER_API __declspec(dllimport)
#endif


namespace qt_mapbox_wrapper
{

struct wrapper;

} // namespace qt_mapbox_wrapper